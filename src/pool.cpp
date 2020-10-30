#include "pool.h"

#include "global_state.h"

namespace mkb2
{

void pool_init()
{
    gs->ball_pool_info.len = MAX_BALLS;
    gs->ball_pool_info.status_list = gs->ball_status_list;

    gs->item_pool_info.len = MAX_ITEMS;
    gs->item_pool_info.status_list = gs->item_status_list;

    gs->stobj_pool_info.len = MAX_STOBJS;
    gs->stobj_pool_info.status_list = gs->stobj_status_list;

    gs->sprite_pool_info.len = MAX_SPRITES;
    gs->sprite_pool_info.status_list = gs->sprite_status_list;

    gs->effect_pool_info.len = MAX_EFFECTS;
    gs->effect_pool_info.status_list = gs->effect_status_list;

    gs->camera_pool_info.len = MAX_CAMERAS;
    gs->camera_pool_info.status_list = gs->camera_status_list;

    pool_clear(&gs->ball_pool_info);
    pool_clear(&gs->item_pool_info);
    pool_clear(&gs->stobj_pool_info);
    pool_clear(&gs->sprite_pool_info);
    pool_clear(&gs->effect_pool_info);
    pool_clear(&gs->camera_pool_info);
}

static void pool_update_idxs(PoolInfo *info, s32 event_id_filter)
{
    s32 new_low_free_idx = -1;
    u32 new_upper_bound = 0;

    if (event_id_filter == EVENT_NONE || gs->events[event_id_filter].status != STAT_NULL)
    {
        for (u32 i = 0; i < info->len; i++)
        {
            if (info->status_list[i] == 0)
            {
                if (new_low_free_idx == -1) new_low_free_idx = i;
            }
            else
            {
                new_upper_bound = i + 1;
            }
        }
    }

    // If there's no free slots, just assume the first slot is free?
    if (new_low_free_idx == -1) new_low_free_idx = 0;

    info->low_free_idx = new_low_free_idx;
    info->upper_bound = new_upper_bound;
}

static void pool_update_idxs_of_all_pools()
{
    pool_update_idxs(&gs->ball_pool_info, EVENT_NONE);
    pool_update_idxs(&gs->item_pool_info, EVENT_ITEM);
    pool_update_idxs(&gs->stobj_pool_info, EVENT_STOBJ);
    pool_update_idxs(&gs->sprite_pool_info, EVENT_SPRITE);
    pool_update_idxs(&gs->effect_pool_info, EVENT_EFFECT);
    pool_update_idxs(&gs->camera_pool_info, EVENT_CAMERA);
}

void pool_tick()
{
    pool_update_idxs_of_all_pools();
    // There's another function call here in the original game that appears to do nothing
}

s32 pool_alloc(PoolInfo *info, u8 status)
{
    // Search for a free idx starting at the "low free idx" (last known lowest free idx)
    // until the end of the list is reached
    for (u32 i = 0; i < info->len; i++)
    {
        if (info->status_list[i] == 0)
        {
            if (info->upper_bound < i + 1) info->upper_bound = i + 1;
            info->low_free_idx = i + 1;

            info->status_list[i] = status;
            return i;
        }
    }

    // If no free slot was found after `low_free_idx`, look for a free slot from the beginning of the list
    // (this would happen if something below `low_free_idx` was freed this frame)
    for (u32 i = 0; i < info->low_free_idx; i++)
    {
        if (info->status_list[i] == 0)
        {
            info->low_free_idx = i + 1;
            info->status_list[i] = status;
            return i;
        }
    }

    // No free slot was found
    return -1;
}

void pool_clear(PoolInfo *info)
{
    for (u32 i = 0; i < info->len; i++)
    {
        info->status_list[i] = 0;
    }

    info->low_free_idx = 0;
    info->upper_bound = 0;
}

}