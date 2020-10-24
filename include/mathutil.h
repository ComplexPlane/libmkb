#pragma once

#include "mathtypes.h"

namespace mkb2
{

// TODO I believe what I call Euler rotations are not really Euler rotations. Come up with different names for these.

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

// TODO
void math_normalize_mtxa_quat();

/*
 * Push Matrix A onto the matrix stack.
 */
void math_push_mtxa();

/*
 * Pop from the matrix stack into Matrix A.
 */
void math_pop_mtxa();

/*
 * Copy the topmost matrix on the matrix stack into Matrix A.
 */
void math_peek_mtxa();

/*
 * Copy Matrix A into the provided `mtx`.
 */
void math_get_mtxa(Mtx *mtx);

/*
 * Copy the provided `mtx` into Matrix A.
 */
void math_set_mtxa(Mtx *mtx);

/*
 * Assign Matrix A to the value of Matrix B.
 */
void math_set_mtxa_mtxb();

/*
 * Assign Matrix B to the value of Matrix A.
 */
void math_set_mtxb_mtxa();

/*
 * Copy the `src` matrix into the `dst` matrix.
 */
void math_copy_mtx(Mtx *src, Mtx *dst);

/*
 * Invert Matrix A.
 */
void math_invert_mtxa();

/*
 * Transpose Matrix A.
 */
void math_transpose_mtxa();

/*
 * Multiply Matrix A by the given `mtx` on the right.
 *
 * mtxa = mtxa * mtx
 */
void math_mult_mtxa_right(Mtx *mtx);

/*
 * Multiple Matrix A by the given `mtx` on the left.
 *
 * mtxa = mtx * mtxa
 */
void math_mult_mtxa_left(Mtx *mtx);

/*
 * Assign Matrix A to the matrix product of Matrix B and `mtx`.
 *
 * mtxa = mtxb * mtx
 */
void math_set_mtxa_mtxb_mult_mtx(Mtx *mtx);

/*
 * Assign `dst` to the matrix product of `mtx1` and `mtx2`.
 */
void math_mult_mtx(Mtx *mtx1, Mtx *mtx2, Mtx *dst);

/*
 * Transform a point by Matrix A, then assign the result to the translation column of Matrix A.
 *
 * Point is given as a Vec3f.
 */
void math_tf_point_by_mtxa_trans_v(Vec3f *point);

/*
 * Transform a point by Matrix A, then assign the result to the translation column of Matrix A.
 *
 * Point is given as x, y, z components.
 */
void math_tf_point_by_mtxa_trans(f32 x, f32 y, f32 z);

/*
 * Transform a point by the inverse of Matrix A,
 * then assign the result to the translation column of Matrix A.
 *
 * Point is given as a Vec3f.
 */
void math_inv_tf_point_by_mtxa_trans_v(Vec3f *point);

/*
 * Transform a point by the inverse of Matrix A,
 * then assign the result to the translation column of Matrix A.
 *
 * Point is given as x, y, z components.
 */
void math_inv_tf_point_by_mtxa_trans(f32 x, f32 y, f32 z);

/*
 * Scale the square part of Matrix A by the vector `scale`.
 *
 * The first, second, and third columns of Matrix A are scaled by
 * `scale.x`, `scale.y`, and `scale.z` respectively.
 */
void math_scale_mtxa_sq_v(Vec3f *scale);

/*
 * Scale every value in the square part of Matrix A by `scale`.
 */
void math_scale_mtxa_sq_s(f32 scale);

/*
 * Scale the square part of Matrix A by `x`, `y`, and `z` scales.
 *
 * The first, second, and third columns of Matrix A are scaled by
 * `x`, `y`, and `z` respectively.
 */
void math_scale_mtxa_sq(f32 x, f32 y, f32 z);

/*
 * Transform a Vec3f point by Matrix A.
 *
 * `src` is the input point and `dst` is the resultant transformed point.
 * Transform includes translation, unlike `math_tf_vec` variants.
 */
void math_tf_point_by_mtxa_v(Vec3f *src, Vec3f *dst);

/*
 * Transform a Vec3f vector by Matrix A.
 *
 * `src` is the input vector and `dst` is the resultant transformed vector.
 * Transforming a vector does not include translation, unlike transforming a point
 * with `math_tf_point` variants.
 */
void math_tf_vec_by_mtxa_v(Vec3f *src, Vec3f *dst);

/*
 * Transform a (x, y, z) point by Matrix A.
 *
 * Transforming a point includes translation, unlike `math_tf_vec` variants.
 */
void math_tf_point_by_mtxa(f32 x, f32 y, f32 z, Vec3f *dst);

/*
 * Transform a (x, y, z) vector by Matrix A.
 *
 * Transforming a vector does not include translation, unlike transforming a point
 * with `math_tf_point` variants.
 */
void math_tf_vec_by_mtxa(f32 x, f32 y, f32 z, Vec3f *dst);

// TODO port the handful of "xz" whatever funcs

void math_tf_point_xz_by_mtxa_v(Vec3f *src, Vec3f *dst);

/*
 * Apply an X rotation to Matrix A.
 *
 * Equivalent to: mtxa = mtxa * xrotation(angle)
 */
void math_mult_mtxa_by_rotate_x(s16 angle);

/*
 * Apply a Y rotation to Matrix A.
 *
 * Equivalent to: mtxa = mtxa * yrotation(angle)
 */
void math_mult_mtxa_by_rotate_y(s16 angle);

/*
 * Apply a Z rotation to Matrix A.
 *
 * Equivalent to: mtxa = mtxa * zrotation(angle)
 */
void math_mult_mtxa_by_rotate_z(s16 angle);

/*
 * Initialize Matrix A from a rotation quaternion.
 *
 * Translation is set to 0.
 */
void math_set_mtxa_rotate_quat(Quat *quat);

/*
 * Multiply two quaternions.
 *
 * `left` is multiplied by `right` and the result is stored in `dst`.
 */
void math_mult_quat(Quat *dst, Quat *left, Quat *right);

/*
 * Initialize a rotation quaternion from Matrix A.
 */
void math_quat_from_mtxa(Quat *out_quat);

/*
 * Initialize a rotation quaternion from an axis and an angle.
 */
void math_quat_from_axis_angle(Quat *out_quat, Vec3f *axis, s16 angle);

/*
 * Extract the rotation axis and angle from a quaternion.
 *
 * Writes the axis to `out_axis` and returns the angle in radians.
 */
double math_quat_to_axis_angle(Quat *quat, Vec3f *out_axis);

/*
 * Normalize a quaternion.
 */
void math_normalize_quat(Quat *quat);

/*
 * Quaternion spherical linear interpolation.
 */
void math_quat_slerp(f32 t, Quat *dst, Quat *quat1, Quat *quat2);

/*
 * Compute a Vec3s Euler rotation from a ray direction.
 *
 * The Z component of the rotation is always 0.
 */
void math_ray_to_euler(Vec3f *ray_start, Vec3f *ray_end, Vec3s *out_rot);

/*
 * Compute an x, y Euler rotation from a ray direction.
 */
void math_ray_to_euler_xy(Vec3f *ray_start, Vec3f *ray_end, s16 *out_rot_x, s16 *out_rot_y);

/*
 * Compute a Vec3s Euler rotation from a vector direction.
 *
 * The Z component of the rotation is always 0.
 */
void math_vec_to_euler(Vec3f *vec, Vec3s *out_rot);

/*
 * Compute an x, y Euler rotation from a vector direction.
 */
void math_vec_to_euler_xy(Vec3f *vec, s16 *out_rot_x, s16 *out_rot_y);

/*
 * Compute a y, x, z Euler rotation from Matrix A's rotation.
 */
void math_mtxa_to_euler(s16 *out_rot_y, s16 *out_rot_x, s16 *out_rot_z);

/*
 * Compute a Vec3f Euler rotation from Matrix A's rotation.
 */
void math_mtxa_to_euler_v(Vec3s *out_rot);

}