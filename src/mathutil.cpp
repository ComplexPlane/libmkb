#include "mathutil.h"

// Needed for M_PI and other constants from <cmath>
#define _USE_MATH_DEFINES

#include <Eigen/Dense>

#include "mathtypes.h"
#include "vecutil.h"
#include "global_state.h"

// TODO implement the equivalent of f32ing-point condition register checks?

namespace mkb2
{

/*
 * Eigen transform matrix type used to wrap the game's Mtx type.
 */
using EigenMtx = Eigen::Transform<f32, 3, Eigen::AffineCompact, Eigen::RowMajor>;

/*
 * Various helpers to convert between Eigen and the game's math types.
 *
 * Conversion performed by copying which is slow; if you find a better way, let me know.
 */

inline EigenMtx emtx_from_mtxa()
{
    EigenMtx emtx;
    memcpy(emtx.data(), gs->mtxa_raw, sizeof(Mtx));
    return emtx;
}

inline void emtx_to_mtxa(const EigenMtx &emtx)
{
    memcpy(gs->mtxa_raw, emtx.data(), sizeof(Mtx));
}

inline EigenMtx emtx_from_mtxb()
{
    EigenMtx emtx;
    memcpy(emtx.data(), gs->mtxb_raw, sizeof(Mtx));
    return emtx;
}

inline void emtx_to_mtxb(const EigenMtx &emtx)
{
    memcpy(gs->mtxb_raw, emtx.data(), sizeof(Mtx));
}

inline EigenMtx emtx_from_mtx(Mtx *mtx)
{
    EigenMtx emtx;
    memcpy(emtx.data(), mtx, sizeof(Mtx));
    return emtx;
}

inline void emtx_to_mtx(const EigenMtx &emtx, Mtx *mtx)
{
    memcpy(mtx, emtx.data(), sizeof(Mtx));
}

inline Eigen::Vector3f evec_from_vec3f(Vec3f *vec)
{
    return Eigen::Vector3f((f32 *) vec);
}

inline void evec_to_vec3f(const Eigen::Vector3f &evec, Vec3f *vec)
{
    vec->x = evec.x();
    vec->y = evec.y();
    vec->z = evec.z();
}

inline Eigen::Quaternionf equat_from_quat(Quat *quat)
{
    return Eigen::Quaternionf((f32 *) quat);
}

inline void equat_to_quat(const Eigen::Quaternionf &equat, Quat *quat)
{
    quat->x = equat.x();
    quat->y = equat.y();
    quat->z = equat.z();
    quat->w = equat.w();
}

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
    return (f32) sin(s16_to_radians(angle));
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
    return radians_to_s16(atan2(y, x));
}

s16 math_atan(f64 x)
{
    return radians_to_s16(atan(x));
}

f32 math_vec_dot_normalized_safe(Vec3f *vec1, Vec3f *vec2)
{
    f32 len1_sq = VEC_LEN_SQ(*vec1);
    f32 len2_sq = VEC_LEN_SQ(*vec2);
    if (len1_sq == INFINITY || len2_sq == INFINITY) return INFINITY;

    f32 dot = VEC_DOT(*vec1, *vec2);
    if (dot == 0.f) return 0.f;

    f32 inv_len_prod = math_rsqrt(len1_sq * len2_sq);
    return dot * inv_len_prod;
}

void math_ray_scale(f32 scale, Vec3f *ray_start, Vec3f *ray_end, Vec3f *out_ray_end)
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

f32 math_vec_dot_normalized(Vec3f *vec1, Vec3f *vec2)
{
    f32 dot = VEC_DOT(*vec1, *vec2);
    f32 len_sq_prod = VEC_LEN_SQ(*vec1) * VEC_LEN_SQ(*vec2);
    f32 inv_len_prod = math_rsqrt(len_sq_prod);
    return dot * inv_len_prod;
}

void math_mtxa_from_identity()
{
    EigenMtx emtx(emtx_from_mtxa());
    emtx.setIdentity();
    emtx_to_mtxa(emtx);
}

void math_mtx_from_identity(Mtx *mtx)
{
    memcpy(mtx, EigenMtx::Identity().data(), sizeof(Mtx));
}

void math_mtxa_sq_from_identity()
{
    EigenMtx emtx(emtx_from_mtxa());
    Eigen::Vector3f tl = emtx.translation();
    emtx.setIdentity();
    emtx.translation() = tl;
    emtx_to_mtxa(emtx);
}

void math_mtxa_tl_from_vec_v(Vec3f *translate)
{
    // TODO
}

void math_mtxa_tl_from_vec(f32 x, f32 y, f32 z)
{
    // TODO
}

void math_mtxa_from_rotate_x(s16 angle)
{
    EigenMtx emtx(Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitX()));
    emtx_to_mtxa(emtx);
}

void math_mtxa_from_rotate_y(s16 angle)
{
    EigenMtx emtx(Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitY()));
    emtx_to_mtxa(emtx);
}

void math_mtxa_from_rotate_z(s16 angle)
{
    EigenMtx emtx(Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitZ()));
    emtx_to_mtxa(emtx);
}

void math_mtxa_from_mtxb_tfset_point_v(Vec3f *point)
{
    math_mtxa_from_mtxb_tfset_point(point->x, point->y, point->z);
}

void math_mtxa_from_mtxb_tfset_point(f32 x, f32 y, f32 z)
{
    Eigen::Vector3f epoint(x, y, z);
    EigenMtx emtxb(emtx_from_mtxb());
    emtxb.translation() = emtxb * epoint;
    emtx_to_mtxa(emtxb);
}

void math_mtxa_sq_normalize()
{
    // TODO what does this actually do
//    EigenMtx emtxa(emtx_from_mtxa());
//    emtxa.linear().normalize();
//    emtx_to_mtxa(emtxa);
}

void math_mtxa_push()
{
    // Check does not appear in the original source
    assert(gs->mtx_stack_ptr <= gs->mtx_stack + MTX_STACK_LEN);

    memcpy(--gs->mtx_stack_ptr, &gs->mtxa_raw, sizeof(Mtx));
}

void math_mtxa_pop()
{
    // Check does not appear in the original source
    assert(gs->mtx_stack_ptr >= gs->mtx_stack);

    memcpy(&gs->mtxa_raw, gs->mtx_stack_ptr++, sizeof(Mtx));
}

void math_mtxa_to_mtx(Mtx *mtx)
{
    memcpy(mtx, &gs->mtxa_raw, sizeof(Mtx));
}

void math_mtxa_from_mtx(Mtx *mtx)
{
    memcpy(&gs->mtxa_raw, mtx, sizeof(Mtx));
}

void math_mtxa_peek()
{
    memcpy(&gs->mtxa_raw, gs->mtx_stack_ptr, sizeof(Mtx));
}

void math_mtxa_from_mtxb()
{
    memcpy(&gs->mtxa_raw, &gs->mtxb_raw, sizeof(Mtx));
}

void math_mtxa_to_mtxb()
{
    memcpy(&gs->mtxb_raw, &gs->mtxa_raw, sizeof(Mtx));
}

void math_mtx_copy(Mtx *src, Mtx *dst)
{
    memcpy(dst, src, sizeof(Mtx));
}

void math_mtxa_invert()
{
    emtx_to_mtxa(emtx_from_mtxa().inverse());
}

void math_mtxa_transpose()
{
    // TODO
}

void math_mtxa_mult_right(Mtx *mtx)
{
    emtx_to_mtxa(emtx_from_mtxa() * emtx_from_mtx(mtx));
}

void math_mtxa_mult_left(Mtx *mtx)
{
    emtx_to_mtxa(emtx_from_mtx(mtx) * emtx_from_mtxa());
}

void math_mtxa_from_mtxb_mult_mtx(Mtx *mtx)
{
    emtx_to_mtxa(emtx_from_mtxb() * emtx_from_mtx(mtx));
}

void math_mtx_mult(Mtx *mtx1, Mtx *mtx2, Mtx *dst)
{
    emtx_to_mtx(emtx_from_mtx(mtx1) * emtx_from_mtx(mtx2), dst);
}

void math_mtxa_tl_from_mtxa_tf_point_v(Vec3f *point)
{
    math_mtxa_tl_from_mtxa_tf_point(point->x, point->y, point->z);
}

void math_mtxa_tl_from_mtxa_tf_point(f32 x, f32 y, f32 z)
{
    Eigen::Vector3f epoint(x, y, z);
    EigenMtx emtx(emtx_from_mtxa());
    emtx.translation() = emtx * epoint;
    emtx_to_mtxa(emtx);
}

void math_mtxa_tl_from_mtxa_inv_tf_point_v(Vec3f *point)
{
    math_mtxa_tl_from_mtxa_inv_tf_point(point->x, point->y, point->z);
}

void math_mtxa_tl_from_mtxa_inv_tf_point(f32 x, f32 y, f32 z)
{
    Eigen::Vector3f epoint(x, y, z);
    EigenMtx emtx(emtx_from_mtxa());
    emtx.translation() = emtx.inverse() * epoint;
    emtx_to_mtxa(emtx);
}

void math_mtxa_mult_scale_v(Vec3f *scale)
{
    math_mtxa_mult_scale(scale->x, scale->y, scale->z);
}

void math_mtxa_mult_scale_s(f32 scale)
{
    math_mtxa_mult_scale(scale, scale, scale);
}

void math_mtxa_mult_scale(f32 x, f32 y, f32 z)
{
    gs->mtxa_raw[0][0] *= x;
    gs->mtxa_raw[1][0] *= x;
    gs->mtxa_raw[2][0] *= x;
    gs->mtxa_raw[0][1] *= y;
    gs->mtxa_raw[1][1] *= y;
    gs->mtxa_raw[2][1] *= y;
    gs->mtxa_raw[0][2] *= z;
    gs->mtxa_raw[1][2] *= z;
    gs->mtxa_raw[2][2] *= z;
}

void math_mtxa_tf_point_v(Vec3f *src, Vec3f *dst)
{
    math_mtxa_tf_point(src->x, src->y, src->z, dst);
}

void math_mtxa_tf_vec_v(Vec3f *src, Vec3f *dst)
{
    math_mtxa_tf_vec(src->x, src->y, src->z, dst);
}

void math_mtxa_tf_point(f32 x, f32 y, f32 z, Vec3f *dst)
{
    evec_to_vec3f(emtx_from_mtxa() * Eigen::Vector3f(x, y, z), dst);
}

void math_mtxa_tf_vec(f32 x, f32 y, f32 z, Vec3f *dst)
{
    evec_to_vec3f(emtx_from_mtxa().linear() * Eigen::Vector3f(x, y, z), dst);
}

void math_tf_point_xz_by_mtxa_v(Vec3f *src, Vec3f *dst)
{
    // TODO implement (also figure out what this even does)
}

void math_mtxa_mult_rotate_x(s16 angle)
{
    emtx_to_mtxa(emtx_from_mtxa() * Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitX()));
}

void math_mtxa_mult_rotate_y(s16 angle)
{
    emtx_to_mtxa(emtx_from_mtxa() * Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitY()));
}

void math_mtxa_mult_rotate_z(s16 angle)
{
    emtx_to_mtxa(emtx_from_mtxa() * Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitZ()));
}

void math_mtxa_from_quat(Quat *quat)
{
    emtx_to_mtxa(EigenMtx(Eigen::Quaternionf((f32 *) quat)));
}

void math_quat_mult(Quat *dst, Quat *left, Quat *right)
{
    equat_to_quat(equat_from_quat(left) * equat_from_quat(right), dst);
}

void math_mtxa_to_quat(Quat *out_quat)
{
    equat_to_quat(Eigen::Quaternionf(emtx_from_mtxa().rotation()), out_quat);
}

void math_quat_from_axis_angle(Quat *out_quat, Vec3f *axis, s16 angle)
{
    // Unnecessary to first convert it to an Eigen transform?
    EigenMtx m(Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f((f32 *) axis)));
    equat_to_quat(Eigen::Quaternionf(m.rotation()), out_quat);
}

double math_quat_to_axis_angle(Quat *quat, Vec3f *out_axis)
{
    Eigen::Vector3f axis(quat->x, quat->y, quat->z);
    axis.normalize();
    evec_to_vec3f(axis, out_axis);
    return 2.0 * acos(quat->w);
}

void math_quat_normalize(Quat *quat)
{
    equat_to_quat(equat_from_quat(quat).normalized(), quat);
}

void math_quat_from_vecs(Quat *out_quat, Vec3f *start, Vec3f *end)
{
    Eigen::Vector3f estart((f32 *) start);
    Eigen::Vector3f eend((f32 *) end);
    Eigen::Quaternionf q(Eigen::Quaternionf::FromTwoVectors(estart, eend));
    equat_to_quat(q, out_quat);
}

void math_quat_slerp(f32 t, Quat *dst, Quat *quat1, Quat *quat2)
{
    Eigen::Quaternionf q1(equat_from_quat(quat1));
    Eigen::Quaternionf q2(equat_from_quat(quat2));
    Eigen::Quaternionf result = q1.slerp(t, q2);
    equat_to_quat(result, dst);
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
    math_mtxa_push();

    Vec3f forward = {0.f, 0.f, -1.f};
    Vec3f up = {0.f, 1.f, 0.f};
    math_mtxa_tf_point_v(&forward, &forward);
    math_mtxa_tf_point_v(&up, &up);

    f32 forward_len2d = math_sqrt(forward.x * forward.x + forward.z * forward.z);
    *out_rot_x = math_atan2(forward.y, forward_len2d);
    // Quick hack to add 180 degrees to the angle?
    // Appears this was achieved in original source's `math_vec_to_euler_xy()` by
    // negating both arguments to `math_atan2()`
    *out_rot_y = math_atan2(forward.x, forward.z) + 0x8000;

    // I think this undoes this X and Y rotation on the up vector, leaving only the Z rotation
    math_mtxa_from_rotate_y(*out_rot_y);
    math_mtxa_mult_rotate_x(*out_rot_x);
    math_tf_point_xz_by_mtxa_v(&up, &up);
    *out_rot_z = -math_atan2(up.x, up.y);

    math_mtxa_pop();
}

void math_mtxa_to_euler_v(Vec3s *out_rot)
{
    // The original game reimplements the logic here instead of calling `math_mtxa_to_euler()`
    math_mtxa_to_euler(&out_rot->y, &out_rot->x, &out_rot->z);
}

}