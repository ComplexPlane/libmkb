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

}
