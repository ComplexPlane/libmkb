#include "pool.h"

#include "ball.h"
#include "item.h"
#include "stobj.h"
#include "sprite.h"
#include "effect.h"
#include "camera.h"
#include "event.h"

namespace mkb2
{

Ball g_balls[MAX_BALLS];
Item g_items[MAX_ITEMS];
StageObject g_stobjs[MAX_STOBJS];
Sprite g_sprites[MAX_SPRITES];
Effect g_effects[MAX_EFFECTS];
Camera g_cameras[MAX_CAMERAS];

void pool_init()
{
    g_ball_pool_info.len = MAX_BALLS;
    g_ball_pool_info.status_list = g_ball_status_list;

    g_item_pool_info.len = MAX_ITEMS;
    g_item_pool_info.status_list = g_item_status_list;

    g_stobj_pool_info.len = MAX_STOBJS;
    g_stobj_pool_info.status_list = g_stobj_status_list;

    g_sprite_pool_info.len = MAX_SPRITES;
    g_sprite_pool_info.status_list = g_sprite_status_list;

    g_effect_pool_info.len = MAX_EFFECTS;
    g_effect_pool_info.status_list = g_effect_status_list;

    g_camera_pool_info.len = MAX_CAMERAS;
    g_camera_pool_info.status_list = g_camera_status_list;

    pool_clear(&g_ball_pool_info);
    pool_clear(&g_item_pool_info);
    pool_clear(&g_stobj_pool_info);
    pool_clear(&g_sprite_pool_info);
    pool_clear(&g_effect_pool_info);
    pool_clear(&g_camera_pool_info);
}

static void pool_update_idxs(PoolInfo *info, s32 event_id_filter)
{
    s32 new_low_free_idx = -1;
    u32 new_upper_bound = 0;

    if (event_id_filter == EVENT_NONE || g_events[event_id_filter].status != STAT_NULL)
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
    pool_update_idxs(&g_ball_pool_info, EVENT_NONE);
    pool_update_idxs(&g_item_pool_info, EVENT_ITEM);
    pool_update_idxs(&g_stobj_pool_info, EVENT_STOBJ);
    pool_update_idxs(&g_sprite_pool_info, EVENT_SPRITE);
    pool_update_idxs(&g_effect_pool_info, EVENT_EFFECT);
    pool_update_idxs(&g_camera_pool_info, EVENT_CAMERA);
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