#pragma once

#include "mathtypes.h"

namespace mkb2
{

/*
 * Initializes the math library.
 */
void math_init();

/*
 * Computes the square-root of `x`.
 *
 * Returns  0 if `x` is non-positive.
 */
f32 math_sqrt(f64 x);

/*
 * Computes the reciprocal square-root of `x` (`1/sqrt(x)`).
 *
 * Returns INFINITY if `x` is non-positive.
 */
f32 math_rsqrt(f64 x);

/*
 * Computes the square-root and reciprocal square-root of `x` simultaneously.
 *
 * Writes `sqrt(x)` (or 0 if `x` is non-positive) to `out_sqrt`,
 * and returns `1/sqrt(x)` (or INFINITY if `x` is non-positive).
 */
f32 math_sqrt_rsqrt(f64 x, f32 *out_sqrt);

/*
 * Computes the sine of `angle`.
 */
f32 math_sin(s16 angle);

/*
 * Computes the sine and cosine of `angle` simultaneously.
 *
 * The sine and cosine values respectively are written to the length-two array pointed to
 * by `out_sin_cos`.
 */
void math_sin_cos_v(s16 angle, f32 *out_sin_cos);

/*
 * Computes the tangent of `angle`.
 */
f32 math_tan(s16 angle);

/*
 * Computes the four quadrant arctangent of `y` and `x` as a s16 angle.
 */
s16 math_atan2(f64 y, f64 x);

/*
 * Computes the arctangent of `x` as a s16 angle.
 */
s16 math_atan(f64 x);

/*
 * Computes the dot product of the normals of two vectors, clamped to [0.f, 1.f].
 *
 * `vec1` and `vec2` need not be normal vectors themselves.
 */
f32 math_dot_normalized_clamp(Vec3f *vec1, Vec3f *vec2);

/*
 * Scales the length of a ray.
 *
 * The start pt remains the same, and the new end pt is written to `out_ray_end`.
 */
void math_scale_ray(f32 scale, Vec3f *ray_start, Vec3f *ray_end, Vec3f *out_ray_end);

/*
 * Sets the length of a vector.
 *
 * Vector with new length is written to `out_vec`.
 */
void math_vec_set_length(f32 len, Vec3f *vec, Vec3f *out_vec);

/*
 * Normalizes a vector and returns its original length.
 */
f32 math_vec_normalize_len(Vec3f *vec);

/*
 * Computes the dot product of the normals of two vectors.
 *
 * `vec1` and `vec2` need not be normal vectors themselves.
 */
f32 math_dot_normalized(Vec3f *vec1, Vec3f *vec2);

/*
 * Sets Matrix A to the identity matrix.
 */
void math_set_mtxa_identity();

/*
 * Sets the passed matrix to the identity matrix.
 */
void math_set_mtx_identity(Mtx *mtx);

// TODO
void math_set_mtxa_identity_sq();

// TODO
void math_set_mtxa_translate_v(Vec3f *translate);

// TODO
void math_set_mtxa_translate(f32 x, f32 y, f32 z);

/*
 * Set Matrix A to a matrix representing an X rotation with the given angle.
 */
void math_set_mtxa_rotate_x(s16 angle);

/*
 * Set Matrix A to a matrix representing a Y rotation with the given angle.
 */
void math_set_mtxa_rotate_y(s16 angle);

/*
 * Set Matrix A to a matrix representing a Z rotation with the given angle.
 */
void math_set_mtxa_rotate_z(s16 angle);

// TODO math_set_mtxa_translate_from_mtx_transform_..... two functions

}