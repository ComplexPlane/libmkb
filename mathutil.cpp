#include "mathutil.h"

#include "mathtypes.h"
#include "vecutil.h"

#define _USE_MATH_DEFINES
#include <cmath>

// TODO implement the equivalent of floating-point condition register checks?

namespace mkb2
{

/*
 * Convert a s16 angle to radians.
 *
 * Not in the original source, only used here to help leverage standard library float-based math functions.
 */
inline f64 s16_to_radians(s16 angle)
{
    return angle * M_PI / 0x8000;
}

/*
 * Convert an angle in radians to a s16 angle.
 *
 * Not in the original source, only used here to help leverage standard library float-based math functions.
 */
inline s16 radians_to_s16(f64 angle_rad)
{
    return angle_rad * 0x8000 / M_PI;
}

void math_init() {}

f32 math_sqrt(f64 x)
{
    if (x > 0.f) return sqrtf((f32) x);
    return 0.f;
}

f32 math_rsqrt(f64 x)
{
    if (x > 0.f) return 1.f / sqrtf((f32) x);
    return INFINITY;
}

f32 math_sqrt_rsqrt(f64 x, f32 *out_sqrt)
{
    if (x > 0.f)
    {
        *out_sqrt = sqrtf((f32) x);
        return 1.f / *out_sqrt;
    }
    *out_sqrt = 0.f;
    return INFINITY;
}

f32 math_sin(s16 angle)
{
    // This is normally performed with a table lookup.
    // TODO verify accuracy
    (f32) sin(s16_to_radians(angle));
}

void math_sin_cos_v(s16 angle, f32 *out_sin_cos)
{
    f64 angle_rad = s16_to_radians(angle);
    out_sin_cos[0] = (f32) sin(angle_rad);
    out_sin_cos[1] = (f32) cos(angle_rad);
}

f32 math_tan(s16 angle)
{
    return tan(s16_to_radians(angle));
}

s16 math_atan2(f64 y, f64 x)
{
    // TODO verify accuracy
    return radians_to_s16(atan2(y, x));
}

s16 math_atan(f64 x)
{
    return radians_to_s16(atan(x));
}

float math_dot_normalized_clamp(Vec3f *vec1, Vec3f *vec2)
{
    float dot = VEC_DOT(*vec1, *vec2);
    float len_sq_prod = VEC_LEN_SQ(*vec1) * VEC_LEN_SQ(*vec2);

    if (dot > 0.f)
    {
        float denom = math_rsqrt(len_sq_prod);
        return dot / denom;
    }
    return 0.f;
}

void math_scale_ray(float scale, Vec3f *ray_start, Vec3f *ray_end, Vec3f *out_ray_end)
{
    Vec3f scaled_delta = VEC_SCALE(scale, VEC_SUB(*ray_end, *ray_start));
    *out_ray_end = VEC_ADD(*ray_start, scaled_delta);
}

void math_vec_set_length(float len, Vec3f *vec, Vec3f *out_vec)
{
    float len_sq = VEC_LEN_SQ(*vec);

    // Original source checks if it's positive, should be unnecessary
    if (len_sq > 0.f)
    {
        float inv_len = math_rsqrt(len_sq);
        float scale = inv_len * len;
        *out_vec = VEC_SCALE(scale, *vec);
    }
    else
    {
        *out_vec = VEC_ZERO;
    }
}

float math_vec_normalize_len(Vec3f *vec)
{
    float len_sq = VEC_LEN_SQ(*vec);
    if (len_sq > 0.f)
    {
        float inv_len = math_rsqrt(len_sq);
        *vec = VEC_SCALE(inv_len, *vec);
        return len_sq * inv_len;
    }

    *vec = VEC_ZERO;
    return 0.f;
}

float math_dot_normalized(Vec3f *vec1, Vec3f *vec2)
{
    float dot = VEC_DOT(*vec1, *vec2);
    float len_sq_prod = VEC_LEN_SQ(*vec1) * VEC_LEN_SQ(*vec2);
    float denom = math_rsqrt(len_sq_prod);
    return dot / denom;
}

}