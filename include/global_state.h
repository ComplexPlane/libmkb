#pragma once

#include <memory>

#include "mathtypes.h"
#include "event.h"
#include "pool.h"
#include "ball.h"
#include "item.h"
#include "stobj.h"
#include "sprite.h"
#include "effect.h"
#include "camera.h"

namespace mkb2
{

/*
 * Yes, all of it. All of the global variables in Super Monkey Ball 2 in one struct.
 *
 * This lets libmkb clients create and trash independent instances of the game, which should
 * make it easier to visualize the game with incomplete functionality, as well as write some test cases.
 *
 * If you can think of a better way to achieve instancing, then let me know.
 */

// Actual size currently unknown
constexpr u32 MTX_STACK_SIZE = 128;

// TODO order data in MKB2 memory order and label memory addresses
struct GlobalState
{
    Event events[NUM_EVENTS];

    Ball balls[MAX_BALLS];
    Item items[MAX_ITEMS];
    Stobj stobjs[MAX_STOBJS];
    Sprite sprites[MAX_SPRITES];
    Effect effects[MAX_EFFECTS];
    Camera cameras[MAX_CAMERAS];

    u8 ball_status_list[MAX_BALLS];
    u8 item_status_list[MAX_ITEMS];
    u8 stobj_status_list[MAX_STOBJS];
    u8 sprite_status_list[MAX_SPRITES];
    u8 effect_status_list[MAX_EFFECTS];
    u8 camera_status_list[MAX_CAMERAS];

    PoolInfo ball_pool_info;
    PoolInfo item_pool_info;
    PoolInfo stobj_pool_info;
    PoolInfo sprite_pool_info;
    PoolInfo effect_pool_info;
    PoolInfo camera_pool_info;

    // Sometimes read by the game directly, usually mtxa/mtxb only written to with `math_` functions though
    Mtx *mtxa = &mtxa_raw;

    /*
     * Data originally stored in the locked cache, starting at 0xE0000000
     */

    Mtx mtxa_raw;
    Mtx mtxb_raw;
    Mtx mtx_stack[MTX_STACK_SIZE]; // Location in locked cache currently unknown
    Mtx *mtx_stack_ptr = mtx_stack + MTX_STACK_SIZE;
};

/*
 * The current instance of GlobalState used by libmkb.
 *
 * Statically-initialized to an instance by default.
 */
extern std::unique_ptr<GlobalState> gs;

}