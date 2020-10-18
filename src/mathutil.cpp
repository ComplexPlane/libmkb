#include "mathutil.h"

#include "mathtypes.h"
#include "vecutil.h"

// It seems that _USE_MATH_DEFINES must be placed above Eigen includes for constants in cmath like M_PI to work
#define _USE_MATH_DEFINES
#include <Eigen/Dense>
#include <cmath>

// TODO implement the equivalent of f32ing-point condition register checks?

namespace mkb2
{

/*
 * Eigen transform matrix type used to wrap the game's Mtx type.
 */
using EigenMtx = Eigen::Transform<f32, 3, Eigen::AffineCompact, Eigen::RowMajor>;

/*
 * Eigen vector map type used to wrap the game's Vec3f type.
 */
using EigenVec3fWrapper = Eigen::Map<Eigen::Vector3f>;

/*
 * Matrix A and Matrix B in the game's matrix format.
 *
 * Computations are performed on EigenMtx equivalents, but the result is coped to these native
 * matrices since the game reads them directly sometimes. If someone can figure out how to wrap
 * Eigen Transform matrices around these without needing to copy, feel free to fix it.
 */
static Mtx s_mtxa; // Matrix A
static Mtx s_mtxb; // Matrix B

/*
 * The matrix stack.
 *
 * I don't know exactly how large the stack is in the locked cache in the original source.
 */
static Mtx s_mtx_stack[128];
static s32 s_mtx_stack_ptr = -1;

/*
 * Eigen wrappers around Matrix A and Matrix B.
 *
 * Only for convenience, these don't exist in the game.
 */

static EigenMtx s_eigen_mtxa;
static EigenMtx s_eigen_mtxb;

/*
 * Convert a s16 angle to radians.
 *
 * Not in the original source, only used here to help leverage standard library f32-based math functions.
 */
inline f64 s16_to_radians(s16 angle)
{
    return angle * M_PI / 0x8000;
}

/*
 * Convert an angle in radians to a s16 angle.
 *
 * Not in the original source, only used here to help leverage standard library f32-based math functions.
 */
inline s16 radians_to_s16(f64 angle_rad)
{
    return angle_rad * 0x8000 / M_PI;
}

/*
 * Copies the Eigen Matrix A to the game's native Matrix A.
 *
 * Not necessary if someone figures out how to make Eigen Transform wrap
 * the game's Matrix A directly.
 */
inline void copy_mtxa()
{
    memcpy(&s_mtxa, s_eigen_mtxa.data(), sizeof(Mtx));
}
inline void copy_mtxb()
{
    memcpy(&s_mtxb, s_eigen_mtxb.data(), sizeof(Mtx));
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

s16 math_atan2(f64 x, f64 y)
{
    // TODO verify accuracy
    return radians_to_s16(atan2(y, x));
}

s16 math_atan(f64 x)
{
    return radians_to_s16(atan(x));
}

f32 math_dot_normalized_clamp(Vec3f *vec1, Vec3f *vec2)
{
    f32 dot = VEC_DOT(*vec1, *vec2);
    f32 len_sq_prod = VEC_LEN_SQ(*vec1) * VEC_LEN_SQ(*vec2);

    if (dot > 0.f)
    {
        f32 denom = math_rsqrt(len_sq_prod);
        return dot / denom;
    }
    return 0.f;
}

void math_scale_ray(f32 scale, Vec3f *ray_start, Vec3f *ray_end, Vec3f *out_ray_end)
{
    Vec3f scaled_delta = VEC_SCALE(scale, VEC_SUB(*ray_end, *ray_start));
    *out_ray_end = VEC_ADD(*ray_start, scaled_delta);
}

void math_vec_set_length(f32 len, Vec3f *vec, Vec3f *out_vec)
{
    f32 len_sq = VEC_LEN_SQ(*vec);

    // Original source checks if it's positive, should be unnecessary
    if (len_sq > 0.f)
    {
        f32 inv_len = math_rsqrt(len_sq);
        f32 scale = inv_len * len;
        *out_vec = VEC_SCALE(scale, *vec);
    }
    else
    {
        *out_vec = VEC_ZERO;
    }
}

f32 math_vec_normalize_len(Vec3f *vec)
{
    f32 len_sq = VEC_LEN_SQ(*vec);
    if (len_sq > 0.f)
    {
        f32 inv_len = math_rsqrt(len_sq);
        *vec = VEC_SCALE(inv_len, *vec);
        return len_sq * inv_len;
    }

    *vec = VEC_ZERO;
    return 0.f;
}

f32 math_dot_normalized(Vec3f *vec1, Vec3f *vec2)
{
    f32 dot = VEC_DOT(*vec1, *vec2);
    f32 len_sq_prod = VEC_LEN_SQ(*vec1) * VEC_LEN_SQ(*vec2);
    f32 denom = math_rsqrt(len_sq_prod);
    return dot / denom;
}

void math_set_mtxa_identity()
{
    s_eigen_mtxa.setIdentity();
    copy_mtxa();
}

void math_set_mtx_identity(Mtx *mtx)
{
    EigenMtx emtx(EigenMtx::Identity());
    memcpy(mtx, emtx.data(), sizeof(Mtx));
}

void math_set_mtxa_identity_sq()
{
    // TODO
}

void math_set_mtxa_translate_v(Vec3f *translate)
{
    // TODO
}

void math_set_mtxa_translate(f32 x, f32 y, f32 z)
{
    // TODO
}

void math_set_mtxa_rotate_x(s16 angle)
{
    s_eigen_mtxa *= Eigen::AngleAxis(s16_to_radians(angle), Eigen::Vector3f::UnitX());
    copy_mtxa();
}

void math_set_mtxa_rotate_y(s16 angle)
{
    s_eigen_mtxa *= Eigen::AngleAxis(s16_to_radians(angle), Eigen::Vector3f::UnitY());
    copy_mtxa();
}

void math_set_mtxa_rotate_z(s16 angle)
{
    s_eigen_mtxa *= Eigen::AngleAxis(s16_to_radians(angle), Eigen::Vector3f::UnitZ());
    copy_mtxa();
}

void mat_normalize_mtxa_quat()
{
    // TODO what does this even do??
}

void math_push_mtxa()
{
    // Check does not appear in the original source
    assert(s_mtx_stack_ptr < sizeof(s_mtx_stack) / sizeof(s_mtx_stack[0]));

    memcpy(&s_mtx_stack[++s_mtx_stack_ptr], s_eigen_mtxa.data(), sizeof(Mtx));
}

void math_pop_mtxa()
{
    // Check does not appear in the original source
    assert(s_mtx_stack_ptr >= 0);

    memcpy(s_eigen_mtxa.data(), &s_mtx_stack[s_mtx_stack_ptr--], sizeof(Mtx));
    copy_mtxa();
}

void math_get_mtxa(Mtx *mtx)
{
    memcpy(mtx, s_eigen_mtxa.data(), sizeof(Mtx));
}

void math_set_mtxa(Mtx *mtx)
{
    memcpy(s_eigen_mtxa.data(), mtx, sizeof(Mtx));
    copy_mtxa();
}

void math_peek_mtxa()
{
    // Check does not appear in the original source
    assert(s_mtx_stack_ptr >= 0);

    memcpy(s_eigen_mtxa.data(), &s_mtx_stack[s_mtx_stack_ptr], sizeof(Mtx));
    copy_mtxa();
}

void math_set_mtxa_mtxb()
{
    s_eigen_mtxa = s_eigen_mtxb;
    copy_mtxa();
}

void math_set_mtxb_mtxa()
{
    s_eigen_mtxb = s_eigen_mtxa;
    copy_mtxb();
}

void math_copy_mtx(Mtx *src, Mtx *dst)
{
    memcpy(dst, src, sizeof(Mtx));
}

void math_invert_mtxa()
{
    s_eigen_mtxa = s_eigen_mtxa.inverse();
    copy_mtxa();
}

void math_transpose_mtxa()
{
    // TODO is this really transpose? How to transpose 3x4 matrix into a 3x4 matrix?
}

void math_mult_mtxa_right(Mtx *mtx)
{
    EigenMtx emtx;
    memcpy(emtx.data(), mtx, sizeof(Mtx));
    s_eigen_mtxa = s_eigen_mtxa * emtx;
    copy_mtxa();
}

void math_mult_mtxa_left(Mtx *mtx)
{
    EigenMtx emtx;
    memcpy(emtx.data(), mtx, sizeof(Mtx));
    s_eigen_mtxa = emtx * s_eigen_mtxa;
    copy_mtxa();
}

void math_set_mtxa_mtxb_mult_mtx(Mtx *mtx)
{
    EigenMtx emtx;
    memcpy(emtx.data(), mtx, sizeof(Mtx));
    s_eigen_mtxa = s_eigen_mtxb * emtx;
    copy_mtxa();
}

void math_mult_mtx(Mtx *mtx1, Mtx *mtx2, Mtx *dst)
{
    EigenMtx emtx1, emtx2;
    memcpy(emtx1.data(), mtx1, sizeof(Mtx));
    memcpy(emtx2.data(), mtx2, sizeof(Mtx));
    emtx1 = emtx1 * emtx2;
    memcpy(dst, emtx1.data(), sizeof(Mtx));
}

void math_tf_point_by_mtxa_trans_v(Vec3f *point)
{
    EigenVec3fWrapper epoint((f32 *) point);
    s_eigen_mtxa.translation() = s_eigen_mtxa * epoint;
    copy_mtxa();
}

void math_tf_point_by_mtxa_trans(f32 x, f32 y, f32 z)
{
    Eigen::Vector3f epoint(x, y, z);
    s_eigen_mtxa.translation() = s_eigen_mtxa * epoint;
    copy_mtxa();
}

void math_inv_tf_point_by_mtxa_trans_v(Vec3f *point)
{
    // Faster way to do this with Eigen?
    s_eigen_mtxa = s_eigen_mtxa.inverse() * EigenVec3fWrapper((f32 *) point);
    copy_mtxa();
}

void math_inv_tf_point_by_mtxa_trans(f32 x, f32 y, f32 z)
{
    s_eigen_mtxa = s_eigen_mtxa.inverse() * Eigen::Vector3f(x, y, z);
    copy_mtxa();
}

void math_scale_mtxa_sq_v(Vec3f *scale)
{
    math_scale_mtxa_sq(scale->x, scale->y, scale->z);
}

void math_scale_mtxa_sq_s(f32 scale)
{
    math_scale_mtxa_sq(scale, scale, scale);
}

void math_scale_mtxa_sq(f32 x, f32 y, f32 z)
{
    s_eigen_mtxa(0, 0) *= x;
    s_eigen_mtxa(1, 0) *= x;
    s_eigen_mtxa(2, 0) *= x;
    s_eigen_mtxa(0, 1) *= y;
    s_eigen_mtxa(1, 1) *= y;
    s_eigen_mtxa(2, 1) *= y;
    s_eigen_mtxa(0, 2) *= z;
    s_eigen_mtxa(1, 2) *= z;
    s_eigen_mtxa(2, 2) *= z;
    copy_mtxa();
}

void math_tf_point_by_mtxa_v(Vec3f *src, Vec3f *dst)
{
    math_tf_point_by_mtxa(src->x, src->y, src->z, dst);
}

void math_tf_vec_by_mtxa_v(Vec3f *src, Vec3f *dst)
{
    math_tf_vec_by_mtxa(src->x, src->y, src->z, dst);
}

void math_tf_point_by_mtxa(f32 x, f32 y, f32 z, Vec3f *dst)
{
    Eigen::Vector3f result = s_eigen_mtxa * Eigen::Vector3f(x, y, z);
    dst->x = result.x();
    dst->y = result.y();
    dst->z = result.z();
}

void math_tf_vec_by_mtxa(f32 x, f32 y, f32 z, Vec3f *dst)
{
    Eigen::Vector3f result = s_eigen_mtxa.linear() * Eigen::Vector3f(x, y, z);
    dst->x = result.x();
    dst->y = result.y();
    dst->z = result.z();
}

void math_mult_mtxa_by_rotate_x(s16 angle)
{
    s_eigen_mtxa *= Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitX());
    copy_mtxa();
}

void math_mult_mtxa_by_rotate_y(s16 angle)
{
    s_eigen_mtxa *= Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitY());
    copy_mtxa();
}

void math_mult_mtxa_by_rotate_z(s16 angle)
{
    s_eigen_mtxa *= Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitZ());
    copy_mtxa();
}

void math_set_mtxa_rotate_quat(Quat *quat)
{
    s_eigen_mtxa = Eigen::Quaternionf((f32 *) quat);
    copy_mtxa();
}

void math_mult_quat(Quat *dst, Quat *left, Quat *right)
{
    Eigen::Quaternionf result = Eigen::Quaternionf((f32* ) left) * Eigen::Quaternionf((f32 *) right);
    dst->w = result.w();
    dst->x = result.x();
    dst->y = result.y();
    dst->z = result.z();
}

void math_quat_from_mtxa(Quat *out_quat)
{
    Eigen::Quaternionf q(s_eigen_mtxa.rotation());
    out_quat->w = q.w();
    out_quat->x = q.x();
    out_quat->y = q.y();
    out_quat->z = q.z();
}

void math_quat_from_axis_angle(Quat *out_quat, Vec3f *axis, s16 angle)
{
    // Unnecessary to first convert it to an Eigen transform?
    EigenMtx m(Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f((f32 *) axis)));
    Eigen::Quaternionf q(m.rotation());
    out_quat->w = q.w();
    out_quat->x = q.x();
    out_quat->y = q.y();
    out_quat->z = q.z();
}

double math_quat_to_axis_angle(Quat *quat, Vec3f *out_axis)
{
    Eigen::Vector3f axis(quat->x, quat->y, quat->z);
    axis.normalize();
    out_axis->x = axis.x();
    out_axis->y = axis.y();
    out_axis->z = axis.z();
    return 2.0 * acos(quat->w);
}

void math_normalize_quat(Quat *quat)
{
    Eigen::Quaternionf q((f32 *) quat);
    q.normalize();
    quat->w = q.w();
    quat->x = q.x();
    quat->y = q.y();
    quat->z = q.z();
}

void math_quat_slerp(f32 t, Quat *dst, Quat *quat1, Quat *quat2)
{
    Eigen::Quaternionf q1((f32 *) quat1), q2((f32 *) quat2);
    Eigen::Quaternionf result = q1.slerp(t, q2);
    dst->w = result.w();
    dst->x = result.x();
    dst->y = result.y();
    dst->z = result.z();
}

void math_ray_to_euler(Vec3f *ray_start, Vec3f *ray_end, Vec3s *out_rot)
{
    // The original game reimplements the logic here instead of calling `math_vec_to_euler()`
   Vec3f vec = VEC_SUB(*ray_end, *ray_start);
   math_vec_to_euler(&vec, out_rot);
}

void math_ray_to_euler_xy(Vec3f *ray_start, Vec3f *ray_end, s16 *out_rot_x, s16 *out_rot_y)
{
    // The original game reimplements the logic here instead of calling `math_vec_to_euler_xy()`
    Vec3f vec = VEC_SUB(*ray_end, *ray_start);
    math_vec_to_euler_xy(&vec, out_rot_x, out_rot_y);
}

void math_vec_to_euler(Vec3f *vec, Vec3s *out_rot)
{
    // The original game reimplements the logic here instead of calling `math_vec_to_euler_xy()`
    math_vec_to_euler_xy(vec, &out_rot->x, &out_rot->y);
    out_rot->z = 0.f;
}

void math_vec_to_euler_xy(Vec3f *vec, s16 *out_rot_x, s16 *out_rot_y)
{
    f32 len2d = math_sqrt(vec->x * vec->x + vec->z * vec->z);
    *out_rot_y = math_atan2(vec->y, len2d);
    *out_rot_x = math_atan2(-vec->x, -vec->z);
}

void math_mtxa_to_euler(s16 *out_rot_y, s16 *out_rot_x, s16 *out_rot_z)
{
    math_push_mtxa();

    Vec3f forward = {0.f, 0.f, -1.f};
    Vec3f up = {0.f, 1.f, 0.f};
    math_tf_point_by_mtxa_v(&forward, &forward);
    math_tf_point_by_mtxa_v(&up, &up);

    f32 forward_len2d = math_sqrt(forward.x * forward.x + forward.z * forward.z);
    *out_rot_x = math_atan2(forward.y, forward_len2d);
    // Quick hack to add 180 degrees to the angle?
    // Appears this was achieved in original source's `math_vec_to_euler_xy()` by
    // negating both arguments to `math_atan2()`
    *out_rot_y = math_atan2(forward.x, forward.z) + 0x8000;

    // I think this undoes this X and Y rotation on the up vector, leaving only the Z rotation
    math_set_mtxa_rotate_y(*out_rot_y);
    math_mult_mtxa_by_rotate_x(*out_rot_x);
    math_tf_point_xz_by_mtxa_v(&up, &up);
    *out_rot_z = -math_atan2(up.x, up.y);

    math_pop_mtxa();
}

void math_mtxa_to_euler_v(Vec3s *out_rot)
{
    // The original game reimplements the logic here instead of calling `math_mtxa_to_euler()`
    math_mtxa_to_euler(&out_rot->y, &out_rot->x, &out_rot->z);
}

}