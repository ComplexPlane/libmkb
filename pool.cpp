#include "pool.h"

namespace mkb2
{

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

void pool_tick()
{

}

s32 pool_alloc(PoolInfo *info, u8 status)
{
    return 0;
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