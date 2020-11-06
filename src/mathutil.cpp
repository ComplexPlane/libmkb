/*
 * See mathutil.h for most documentation on the math utility library.
 *
 * This math library in the original game was originally implemented with hand-written Gekko PowerPC assembly.
 * Here in libmkb, we reimplement it in C++ with the help of the standard library as well the Eigen
 * linear algebra library. Library functions should be mathematically equivalent and produce very close to identical
 * results as the original game; unfortunately they cannot perfectly match as floating-point determinism is very hard
 * to achieve cross-platform.
 */

// TODO check more NAN/INF cases?
// TODO implement trig functions using SMB's trig tables for improved accuracy?

#include "mathutil.h"

// Needed for M_PI and other constants from <cmath>
#define _USE_MATH_DEFINES

#include <Eigen/Dense>

#include "mathtypes.h"
#include "vecutil.h"
#include "global_state.h"

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

f32 vec_dot_normalized_safe(Vec3f *vec1, Vec3f *vec2)
{
    f32 len1_sq = VEC_LEN_SQ(*vec1);
    f32 len2_sq = VEC_LEN_SQ(*vec2);
    if (len1_sq == INFINITY || len2_sq == INFINITY) return INFINITY;

    f32 dot = VEC_DOT(*vec1, *vec2);
    if (dot == 0.f) return 0.f;

    f32 inv_len_prod = math_rsqrt(len1_sq * len2_sq);
    return dot * inv_len_prod;
}

void ray_scale(f32 scale, Vec3f *ray_start, Vec3f *ray_end, Vec3f *out_ray_end)
{
    Vec3f scaled_delta = VEC_SCALE(scale, VEC_SUB(*ray_end, *ray_start));
    *out_ray_end = VEC_ADD(*ray_start, scaled_delta);
}

void vec_set_len(f32 len, Vec3f *vec, Vec3f *out_vec)
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

f32 vec_normalize_len(Vec3f *vec)
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

f32 vec_dot_normalized(Vec3f *vec1, Vec3f *vec2)
{
    f32 dot = VEC_DOT(*vec1, *vec2);
    f32 len_sq_prod = VEC_LEN_SQ(*vec1) * VEC_LEN_SQ(*vec2);
    f32 inv_len_prod = math_rsqrt(len_sq_prod);
    return dot * inv_len_prod;
}

void mtxa_from_identity()
{
    EigenMtx emtx(emtx_from_mtxa());
    emtx.setIdentity();
    emtx_to_mtxa(emtx);
}

void mtx_from_identity(Mtx *mtx)
{
    memcpy(mtx, EigenMtx::Identity().data(), sizeof(Mtx));
}

void mtxa_sq_from_identity()
{
    EigenMtx emtx(emtx_from_mtxa());
    Eigen::Vector3f tl = emtx.translation();
    emtx.setIdentity();
    emtx.translation() = tl;
    emtx_to_mtxa(emtx);
}

void mtxa_from_translate(Vec3f *translate)
{
    mtxa_from_translate_xyz(translate->x, translate->y, translate->z);
}

void mtxa_from_translate_xyz(f32 x, f32 y, f32 z)
{
    EigenMtx emtxa(emtx_from_mtxa());
    emtxa.linear().setIdentity();
    emtxa.translation() = Eigen::Vector3f(x, y, z);
    emtx_to_mtxa(emtxa);
}

void mtxa_from_rotate_x(s16 angle)
{
    EigenMtx emtx(Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitX()));
    emtx_to_mtxa(emtx);
}

void mtxa_from_rotate_y(s16 angle)
{
    EigenMtx emtx(Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitY()));
    emtx_to_mtxa(emtx);
}

void mtxa_from_rotate_z(s16 angle)
{
    EigenMtx emtx(Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitZ()));
    emtx_to_mtxa(emtx);
}

void mtxa_from_mtxb_translate(Vec3f *point)
{
    mtxa_from_mtxb_translate_xyz(point->x, point->y, point->z);
}

void mtxa_from_mtxb_translate_xyz(f32 x, f32 y, f32 z)
{
    Eigen::Vector3f epoint(x, y, z);
    EigenMtx emtxb(emtx_from_mtxb());
    emtxb.translation() = emtxb * epoint;
    emtx_to_mtxa(emtxb);
}

void mtxa_normalize_basis()
{
    EigenMtx emtxa(emtx_from_mtxa());
    emtxa.linear().col(0).normalize();
    emtxa.linear().col(1).normalize();
    emtxa.linear().col(2).normalize();
    emtx_to_mtxa(emtxa);
}

void mtxa_push()
{
    // Assertions do not appear in the original source
    assert(gs->mtx_stack_ptr > gs->mtx_stack);
    assert(gs->mtx_stack_ptr <= gs->mtx_stack + MTX_STACK_LEN);

    memcpy(--gs->mtx_stack_ptr, &gs->mtxa_raw, sizeof(Mtx));
}

void mtxa_pop()
{
    // Assertions do not appear in the original source
    assert(gs->mtx_stack_ptr >= gs->mtx_stack);
    assert(gs->mtx_stack_ptr < gs->mtx_stack + MTX_STACK_LEN);

    memcpy(&gs->mtxa_raw, gs->mtx_stack_ptr++, sizeof(Mtx));
}

void mtxa_to_mtx(Mtx *mtx)
{
    memcpy(mtx, &gs->mtxa_raw, sizeof(Mtx));
}

void mtxa_from_mtx(Mtx *mtx)
{
    memcpy(&gs->mtxa_raw, mtx, sizeof(Mtx));
}

void mtxa_peek()
{
    // Assertions do not appear in the original source
    assert(gs->mtx_stack_ptr >= gs->mtx_stack);
    assert(gs->mtx_stack_ptr < gs->mtx_stack + MTX_STACK_LEN);

    memcpy(&gs->mtxa_raw, gs->mtx_stack_ptr, sizeof(Mtx));
}

void mtxa_sq_to_mtx(Mtx *mtx)
{
    EigenMtx emtx(emtx_from_mtx(mtx));
    EigenMtx emtxa(emtx_from_mtxa());
    emtx.linear() = emtxa.linear();
    emtx_to_mtx(emtx, mtx);
}

void mtxa_sq_from_mtx(Mtx *mtx)
{
    EigenMtx emtx(emtx_from_mtx(mtx));
    EigenMtx emtxa(emtx_from_mtxa());
    emtxa.linear() = emtx.linear();
    emtx_to_mtxa(emtxa);
}

void mtxa_from_mtxb()
{
    memcpy(&gs->mtxa_raw, &gs->mtxb_raw, sizeof(Mtx));
}

void mtxa_to_mtxb()
{
    memcpy(&gs->mtxb_raw, &gs->mtxa_raw, sizeof(Mtx));
}

void mtx_copy(Mtx *src, Mtx *dst)
{
    memcpy(dst, src, sizeof(Mtx));
}

void mtxa_invert()
{
    emtx_to_mtxa(emtx_from_mtxa().inverse());
}

void mtxa_transpose()
{
    EigenMtx emtxa(emtx_from_mtxa());
    emtxa.linear().transposeInPlace();
    emtxa.translation() = -(emtxa.linear() * emtxa.translation());
    emtx_to_mtxa(emtxa);
}

void mtxa_mult_right(Mtx *mtx)
{
    emtx_to_mtxa(emtx_from_mtxa() * emtx_from_mtx(mtx));
}

void mtxa_mult_left(Mtx *mtx)
{
    emtx_to_mtxa(emtx_from_mtx(mtx) * emtx_from_mtxa());
}

void mtxa_from_mtxb_mult_mtx(Mtx *mtx)
{
    emtx_to_mtxa(emtx_from_mtxb() * emtx_from_mtx(mtx));
}

void mtx_mult(Mtx *mtx1, Mtx *mtx2, Mtx *dst)
{
    emtx_to_mtx(emtx_from_mtx(mtx1) * emtx_from_mtx(mtx2), dst);
}

void mtxa_translate(Vec3f *point)
{
    mtxa_translate_xyz(point->x, point->y, point->z);
}

void mtxa_translate_xyz(f32 x, f32 y, f32 z)
{
    Eigen::Vector3f epoint(x, y, z);
    EigenMtx emtx(emtx_from_mtxa());
    emtx.translation() = emtx * epoint;
    emtx_to_mtxa(emtx);
}

void mtxa_translate_neg(Vec3f *point)
{
    mtxa_translate_xyz(-point->x, -point->y, -point->z);
}

void mtxa_translate_neg_xyz(f32 x, f32 y, f32 z)
{
    mtxa_translate_xyz(-x, -y, -z);
}

void mtxa_scale(Vec3f *scale)
{
    mtxa_scale_xyz(scale->x, scale->y, scale->z);
}

void mtxa_scale_s(f32 scale)
{
    mtxa_scale_xyz(scale, scale, scale);
}

void mtxa_scale_xyz(f32 x, f32 y, f32 z)
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

void mtxa_tf_point(Vec3f *src, Vec3f *dst)
{
    mtxa_tf_point_xyz(src->x, src->y, src->z, dst);
}

void mtxa_tf_vec(Vec3f *src, Vec3f *dst)
{
    mtxa_tf_vec_xyz(src->x, src->y, src->z, dst);
}

void mtxa_tf_point_xyz(f32 x, f32 y, f32 z, Vec3f *dst)
{
    evec_to_vec3f(emtx_from_mtxa() * Eigen::Vector3f(x, y, z), dst);
}

void mtxa_tf_vec_xyz(f32 x, f32 y, f32 z, Vec3f *dst)
{
    evec_to_vec3f(emtx_from_mtxa().linear() * Eigen::Vector3f(x, y, z), dst);
}

void mtxa_rigid_inv_tf_point(Vec3f *src, Vec3f *dst)
{
    mtxa_rigid_inv_tf_point_xyz(src->x, src->y, src->z, dst);
}

void mtxa_rigid_inv_tf_point_xyz(f32 x, f32 y, f32 z, Vec3f *dst)
{
    f32 mtxa_x = gs->mtxa_raw[0][3];
    f32 mtxa_y = gs->mtxa_raw[1][3];
    f32 mtxa_z = gs->mtxa_raw[2][3];
    mtxa_rigid_inv_tf_vec_xyz(x - mtxa_x, y - mtxa_y, z - mtxa_z, dst);
}

void mtxa_rigid_inv_tf_tl(Vec3f *dst)
{
    f32 x = gs->mtxa_raw[0][3];
    f32 y = gs->mtxa_raw[1][3];
    f32 z = gs->mtxa_raw[2][3];
    mtxa_rigid_inv_tf_vec_xyz(x, y, z, dst);
    dst->x = -dst->x;
    dst->y = -dst->y;
    dst->z = -dst->z;
}

void mtxa_rigid_inv_tf_vec(Vec3f *src, Vec3f *dst)
{
    mtxa_rigid_inv_tf_vec_xyz(src->x, src->y, src->z, dst);
}

void mtxa_rigid_inv_tf_vec_xyz(f32 x, f32 y, f32 z, Vec3f *dst)
{
    EigenMtx emtxa(emtx_from_mtxa());
    Eigen::Vector3f evec(x, y, z);
    evec = emtxa.linear().transpose() * evec;
    evec_to_vec3f(evec, dst);
}

void mtxa_rotate_x(s16 angle)
{
    emtx_to_mtxa(emtx_from_mtxa() * Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitX()));
}

void mtxa_rotate_y(s16 angle)
{
    emtx_to_mtxa(emtx_from_mtxa() * Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitY()));
}

void mtxa_rotate_z(s16 angle)
{
    emtx_to_mtxa(emtx_from_mtxa() * Eigen::AngleAxisf(s16_to_radians(angle), Eigen::Vector3f::UnitZ()));
}

void mtxa_from_quat(Quat *quat)
{
    emtx_to_mtxa(EigenMtx(Eigen::Quaternionf((f32 *) quat)));
}

void quat_mult(Quat *dst, Quat *left, Quat *right)
{
    equat_to_quat(equat_from_quat(left) * equat_from_quat(right), dst);
}

void mtxa_to_quat(Quat *out_quat)
{
    equat_to_quat(Eigen::Quaternionf(emtx_from_mtxa().rotation()), out_quat);
}

void quat_from_axis_angle(Quat *out_quat, Vec3f *axis, s16 angle)
{
//    // Unnecessary to first convert it to an Eigen transform?
//    Eigen::Vector3f evec(evec_from_vec3f(axis));
//    evec.normalize();
//    EigenMtx m(Eigen::AngleAxisf(s16_to_radians(angle), evec));
//    equat_to_quat(Eigen::Quaternionf(m.rotation()), out_quat);

    f32 coeff = math_rsqrt(VEC_LEN_SQ(*axis)) * math_sin(angle / 2);
    out_quat->x = axis->x * coeff;
    out_quat->y = axis->y * coeff;
    out_quat->z = axis->z * coeff;
    out_quat->w = math_sin(angle / 2 + 0x4000);
}

double quat_to_axis_angle(Quat *quat, Vec3f *out_axis)
{
    Eigen::Vector3f axis(quat->x, quat->y, quat->z);
    axis.normalize();
    evec_to_vec3f(axis, out_axis);
    return 2.0 * acos(quat->w);
}

void quat_normalize(Quat *quat)
{
    equat_to_quat(equat_from_quat(quat).normalized(), quat);
}

void quat_from_dirs(Quat *out_quat, Vec3f *start, Vec3f *end)
{
    Eigen::Vector3f estart((f32 *) start);
    Eigen::Vector3f eend((f32 *) end);
    Eigen::Quaternionf q(Eigen::Quaternionf::FromTwoVectors(estart, eend));
    equat_to_quat(q, out_quat);
}

void quat_slerp(f32 t, Quat *dst, Quat *quat1, Quat *quat2)
{
    Eigen::Quaternionf q1(equat_from_quat(quat1));
    Eigen::Quaternionf q2(equat_from_quat(quat2));
    Eigen::Quaternionf result = q1.slerp(t, q2);
    equat_to_quat(result, dst);
}

void ray_to_euler(Vec3f *ray_start, Vec3f *ray_end, Vec3s *out_rot)
{
    // The original game reimplements the logic here instead of calling `vec_to_euler()`
    Vec3f vec = VEC_SUB(*ray_end, *ray_start);
    vec_to_euler(&vec, out_rot);
}

void ray_to_euler_xy(Vec3f *ray_start, Vec3f *ray_end, s16 *out_rot_x, s16 *out_rot_y)
{
    // The original game reimplements the logic here instead of calling `vec_to_euler_xy()`
    Vec3f vec = VEC_SUB(*ray_end, *ray_start);
    vec_to_euler_xy(&vec, out_rot_x, out_rot_y);
}

void vec_to_euler(Vec3f *vec, Vec3s *out_rot)
{
    // The original game reimplements the logic here instead of calling `vec_to_euler_xy()`
    vec_to_euler_xy(vec, &out_rot->x, &out_rot->y);
    out_rot->z = 0;
}

void vec_to_euler_xy(Vec3f *vec, s16 *out_rot_x, s16 *out_rot_y)
{
    f32 len2d = sqrt(vec->x * vec->x + vec->z * vec->z);
    *out_rot_x = math_atan2(vec->y, len2d);
    *out_rot_y = math_atan2(-vec->x, -vec->z);
}

void mtxa_to_euler_yxz(s16 *out_rot_y, s16 *out_rot_x, s16 *out_rot_z)
{
    mtxa_push();

    Vec3f forward = {0.f, 0.f, -1.f};
    Vec3f up = {0.f, 1.f, 0.f};
    mtxa_tf_vec(&forward, &forward);
    mtxa_tf_vec(&up, &up);

    f32 forward_len2d = math_sqrt(forward.x * forward.x + forward.z * forward.z);
    *out_rot_x = math_atan2(forward.y, forward_len2d);
    // Quick hack to add 180 degrees to the angle?
    // Appears this was achieved in original source's `math_vec_to_euler_xy()` by
    // negating both arguments to `math_atan2()`
    *out_rot_y = math_atan2(forward.x, forward.z) + 0x8000;

    // I think this undoes this X and Y rotation on the up vector, leaving only the Z rotation
    mtxa_from_rotate_y(*out_rot_y);
    mtxa_rotate_x(*out_rot_x);
    mtxa_rigid_inv_tf_vec(&up, &up);
    *out_rot_z = -math_atan2(up.x, up.y);

    mtxa_pop();
}

void mtxa_to_euler(Vec3s *out_rot)
{
    // The original game reimplements the logic here instead of calling `mtxa_to_euler_yxz()`
    mtxa_to_euler_yxz(&out_rot->y, &out_rot->x, &out_rot->z);
}

}