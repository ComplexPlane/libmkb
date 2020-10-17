#include "endian.h"

namespace mkb2
{

// TODO use templates to reduce the amount of duplicated stuff?

static void big_to_native(u32 *big, u32 *native)
{
    u8 *big_bytes = (u8 *) big;
    *native = (big_bytes[3] << 0) | (big_bytes[2] << 8) | (big_bytes[1] << 16) | (big_bytes[0] << 24);
}

static void big_to_native(u16 *big, u16 *native)
{
    u8 *big_bytes = (u8 *) big;
    *native = (big_bytes[1] << 0) | (big_bytes[0] << 8);
}

u32 big_to_native(u32 big)
{
    u32 native;
    big_to_native(&big, &native);
    return native;
}

s32 big_to_native(s32 big)
{
    s32 native;
    big_to_native((u32 *) &big, (u32 *) &native);
    return native;
}

f32 big_to_native(f32 big)
{
    f32 native;
    big_to_native((u32 *) &big, (u32 *) &native);
    return native;
}

u16 big_to_native(u16 big)
{
    u16 native;
    big_to_native(&big, &native);
    return native;
}

s16 big_to_native(s16 big)
{
    s16 native;
    big_to_native((u16 *) &big, (u16 *) &native);
    return native;
}

}