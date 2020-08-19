#pragma once

#include "mathtypes.h"

namespace mkb2
{

constexpr u32 MAX_BALLS = 8;
constexpr u32 MAX_ITEMS = 256;
constexpr u32 MAX_STOBJS = 144;
constexpr u32 MAX_SPRITES = 80;
constexpr u32 MAX_EFFECTS = 512;
constexpr u32 MAX_CAMERAS = 5;

struct PoolInfo
{
    u32 len;
    u32 low_free_idx;
    u32 upper_bound;
    u8 *status_list;
};

// Initialize all object pools
void pool_init();

// Refresh metadata on all object pools
void pool_tick();

// Allocate a new object from the given object pool with the initial given status.
// Returns the index of the new object, or -1 if an object could not be allocated
s32 pool_alloc(PoolInfo *info, u8 status);

// Delete all objects from the given pool
void pool_clear(PoolInfo *info);

extern Ball g_balls[MAX_BALLS];
extern Item g_items[MAX_ITEMS];
extern StageObject g_stobjs[MAX_STOBJS];
extern Sprite g_sprites[MAX_SPRITES];
extern Effect g_effects[MAX_EFFECTS];
extern Camera g_cameras[MAX_CAMERAS];

extern u8 g_ball_status_list[MAX_BALLS];
extern u8 g_item_status_list[MAX_ITEMS];
extern u8 g_stobj_status_list[MAX_STOBJS];
extern u8 g_sprite_status_list[MAX_SPRITES];
extern u8 g_effect_status_list[MAX_EFFECTS];
extern u8 g_camera_status_list[MAX_CAMERAS];

extern PoolInfo g_ball_pool_info;
extern PoolInfo g_item_pool_info;
extern PoolInfo g_stobj_pool_info;
extern PoolInfo g_sprite_pool_info;
extern PoolInfo g_effect_pool_info;
extern PoolInfo g_camera_pool_info;

}
