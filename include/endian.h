#pragma once

/*
 * Endianness conversion utilities.
 *
 * These are only for mediating between PowerPC and native types in this codebase;
 * these aren't used at all in the actual game.
 */

#include "mathtypes.h"

namespace mkb2
{

u32 big_to_native(u32 big);
s32 big_to_native(s32 big);
f32 big_to_native(f32 big);
u16 big_to_native(u16 big);
s16 big_to_native(s16 big);

}