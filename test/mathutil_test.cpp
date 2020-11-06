#include <catch.hpp>

#include "mathutil.h"
#include "global_state.h"

using namespace mkb2;

// Useful unions for comparing exact floating-point values with the original game

union Uf
{
    u32 u;
    f32 f;
};

union Ud
{
    u64 u;
    f64 d;
};

union Uf32
{
    u32 u;
    f32 f;
};

union Uf64
{
    u64 u;
    f64 f;
};

union Ufarr32
{
    u32 u[2];
    f32 f[2];
};

union Ufmtx
{
    u32 u[12];
    Mtx f;
};

union Ufvec
{
    u32 u[3];
    Vec3f f;
};

union Ufquat
{
    u32 u[4];
    Quat f;
};

void check_mtx(const Mtx *result, const Mtx *expected)
{
    CHECK((*result)[0][0] == Approx((*expected)[0][0]));
    CHECK((*result)[0][1] == Approx((*expected)[0][1]));
    CHECK((*result)[0][2] == Approx((*expected)[0][2]));
    CHECK((*result)[0][3] == Approx((*expected)[0][3]));

    CHECK((*result)[1][0] == Approx((*expected)[1][0]));
    CHECK((*result)[1][1] == Approx((*expected)[1][1]));
    CHECK((*result)[1][2] == Approx((*expected)[1][2]));
    CHECK((*result)[1][3] == Approx((*expected)[1][3]));

    CHECK((*result)[2][0] == Approx((*expected)[2][0]));
    CHECK((*result)[2][1] == Approx((*expected)[2][1]));
    CHECK((*result)[2][2] == Approx((*expected)[2][2]));
    CHECK((*result)[2][3] == Approx((*expected)[2][3]));
}

void check_mtxa(const Ufmtx &ufmtx)
{
    check_mtx(&gs->mtxa_raw, &ufmtx.f);
}

void check_mtxb(const Ufmtx &ufmtx)
{
    check_mtx(&gs->mtxb_raw, &ufmtx.f);
}

void check_vec3f(Vec3f *result, Vec3f *expected)
{
    if (isnan(expected->x)) CHECK(isnan(result->x)); else CHECK(result->x == Approx(expected->x));
    if (isnan(expected->y)) CHECK(isnan(result->y)); else CHECK(result->y == Approx(expected->y));
    if (isnan(expected->z)) CHECK(isnan(result->z)); else CHECK(result->z == Approx(expected->z));
}

void check_quat(Quat *result, Quat *expected)
{
    if (isnan(expected->x)) CHECK(isnan(result->x)); else CHECK(result->x == Approx(expected->x));
    if (isnan(expected->y)) CHECK(isnan(result->y)); else CHECK(result->y == Approx(expected->y));
    if (isnan(expected->z)) CHECK(isnan(result->z)); else CHECK(result->z == Approx(expected->z));
    if (isnan(expected->w)) CHECK(isnan(result->w)); else CHECK(result->w == Approx(expected->w));
}

/*
 * Initialize Matrix A with dummy values to help detect cases where a function erroneously uses
 * prior values from Matrix A.
 *
 * Originally generated from this code:
 *     Eigen::Vector3f axis{-0.420523f, 0.84720234f, 0.286542f};
 *     axis.normalize();
 *     EigenMtx emtx(Eigen::Quaternionf(Eigen::AngleAxisf(M_PI * 2.f * -0.72062f, axis)));
 *     emtx.translation() = Eigen::Vector3f(25.34604f, -0.543234f, -43.0942f);
 */
void load_dummy_mtxa()
{
    Ufmtx mtx = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41cac4b1,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf0b1162,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22c6076,
    };
    memcpy(gs->mtxa_raw, mtx.f, sizeof(Mtx));
}

/*
 * Same for Matrix B.
 *
 * Originally generated from this code:
 *     Eigen::Vector3f axis{14.495038617319075f, 3.9779715228500434f, -9.773889687710597f};
 *     axis.normalize();
 *     EigenMtx emtx(Eigen::Quaternionf(Eigen::AngleAxisf(M_PI * 2.f * 0.4219873579966982f, axis)));
 *     emtx.translation() = Eigen::Vector3f(14.454696530485036f, -6.624428647521574f, -0.016995047483071568f);
 */
void load_dummy_mtxb()
{
    Ufmtx mtx = {
        0x3eb22ba4, 0x3f182165, 0xbf399fa9, 0x41674670,
        0x3da5dd28, 0xbf4a230c, 0xbf1bb6bf, 0xc0d3fb52,
        0xbf6f1a37, 0x3e1c9d17, 0xbea55310, 0xbc8b3933,
    };
    memcpy(gs->mtxb_raw, mtx.f, sizeof(Mtx));
}

Ufmtx gen_dummy_ufmtx1()
{
    return {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41cac4b1,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf0b1162,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22c6076,
    };
}

Ufmtx gen_dummy_ufmtx2()
{
    return {
        0x3eb22ba4, 0x3f182165, 0xbf399fa9, 0x41674670,
        0x3da5dd28, 0xbf4a230c, 0xbf1bb6bf, 0xc0d3fb52,
        0xbf6f1a37, 0x3e1c9d17, 0xbea55310, 0xbc8b3933,
    };
}

TEST_CASE("math_sqrt()", "[mathutil]")
{
    Uf64 a, b;
    Uf32 c, d, e, f;

    // Positive square-root
    a.u = 0x404d9f62d83c6c98;
    c.u = 0x40f64e91;
    CHECK(math_sqrt(a.f) == Approx(c.f));

    // Negative square-root
    CHECK(math_sqrt(-523.3234) == Approx(0.f));
}

TEST_CASE("math_rsqrt()", "[mathutil]")
{
    Uf64 a, b;
    Uf32 c, d, e, f;

    // Positive values are correct
    a.u = 0x404d9f62d83c6c98;
    c.u = 0x3e05098b;
    CHECK(math_rsqrt(a.f) == Approx(c.f));

    // Negative values return INFINITY
    a.u = 0xc0469e2339c0ebee;
    c.u = 0x7f800000;
    CHECK(math_rsqrt(a.f) == Approx(c.f));
}

TEST_CASE("math_sqrt_rsqrt()", "[mathutil]")
{
    Uf64 a, b;
    Uf32 c, d, e, f;

    // Positive input
    a.u = 0x404d9f62d83c6c98;
    c.u = 0x40f64e91;
    d.u = 0x3e05098b;
    f.f = math_sqrt_rsqrt(a.f, &e.f);
    CHECK(c.f == Approx(e.f));
    CHECK(d.f == Approx(f.f));

    // Negative input
    a.u = 0xc0469e2339c0ebee;
    c.u = 0x0;
    d.u = 0x7f800000;
    f.f = math_sqrt_rsqrt(a.f, &e.f);
    CHECK(c.f == Approx(e.f));
    CHECK(d.f == Approx(f.f));
}

TEST_CASE("math_sin()", "[mathutil]")
{
    Uf32 a, b;
    s16 angle;

    // math_sin
    angle = 0x35a2;
    a.u = 0x3f77c1e1;
    CHECK(math_sin(angle) == Approx(a.f));
}

TEST_CASE("math_atan2()", "[mathutil]")
{
    Uf64 a, b;
    s16 angle;

    angle = 0x2f80;
    f32 sin_cos[2];
    math_sin_cos_v(angle, sin_cos);
    CHECK(math_atan2(sin_cos[0], sin_cos[1]) == angle);

    a.u = 0x3fdaa458312d8777;
    b.u = 0x3fed1876531661e3;
    angle = 0x117e;
    CHECK(math_atan2(a.f, b.f) == angle);

    a.u = 0xbfdaa458312d8777;
    b.u = 0x3fed1876531661e3;
    angle = 0xee82;
    CHECK(math_atan2(a.f, b.f) == angle);

    // Unfortunately these last two checks are not accurate to the exact s16 angle...
    // are we just unlucky, or is there anything we could do to improve robustness?

    a.u = 0xbfdaa458312d8777;
    b.u = 0xbfed1876531661e3;
    angle = 0x917e;
    CHECK(abs(math_atan2(a.f, b.f) - angle) <= 1);

    a.u = 0x3fdaa458312d8777;
    b.u = 0xbfed1876531661e3;
    angle = 0x6e82;
    CHECK(abs(math_atan2(a.f, b.f) - angle) <= 1);
}

TEST_CASE("math_sin_cos_v()", "[mathutil]")
{
    Ufarr32 arr1, arr2;
    s16 angle;

    angle = 0x35a2;
    arr2.u[0] = 0x3f77c1e1;
    arr2.u[1] = 0x3e80e06e;
    math_sin_cos_v(angle, arr1.f);
    CHECK(arr1.f[0] == Approx(arr2.f[0]));
    CHECK(arr1.f[0] == Approx(arr2.f[0]));
}

TEST_CASE("vec_dot_normalized_*()", "[mathutil]")
{
    Ufvec vec1, vec2, vec3, vec4, vec5;
    vec1.f = {-0.5f, -1.f, 0.3};
    vec2.f = {-0.25f, -1.83f, 2.032f};
    vec3.f = {0.25f, 1.83f, -2.032f};
    vec4.f = {0.f, 0.f, 0.f};
    vec5.f = {INFINITY, 0.5, -INFINITY};

    Uf result, expected;

    result.f = vec_dot_normalized_safe(&vec1.f, &vec2.f);
    expected.u = 0x3f4e8add;
    CHECK(result.f == Approx(expected.f));

    result.f = vec_dot_normalized_safe(&vec1.f, &vec3.f);
    expected.u = 0xbf4e8add;
    CHECK(result.f == Approx(expected.f));

    result.f = vec_dot_normalized_safe(&vec1.f, &vec4.f);
    expected.u = 0x00000000;
    CHECK(result.f == Approx(expected.f));

    result.f = vec_dot_normalized_safe(&vec1.f, &vec5.f);
    expected.u = 0x7f800000;
    CHECK(result.f == Approx(expected.f));

    result.f = vec_dot_normalized_safe(&vec4.f, &vec4.f);
    expected.u = 0x00000000;
    CHECK(result.f == Approx(expected.f));

    result.f = vec_dot_normalized(&vec1.f, &vec2.f);
    expected.u = 0x3f4e8add;
    CHECK(result.f == Approx(expected.f));

    result.f = vec_dot_normalized(&vec1.f, &vec3.f);
    expected.u = 0xbf4e8add;
    CHECK(result.f == Approx(expected.f));

    result.f = vec_dot_normalized(&vec1.f, &vec4.f);
    expected.u = 0xffc00000;
    CHECK(isnan(result.f));

    result.f = vec_dot_normalized(&vec1.f, &vec5.f);
    expected.u = 0xffc00000;
    CHECK(isnan(result.f));

    result.f = vec_dot_normalized(&vec4.f, &vec4.f);
    expected.u = 0xffc00000;
    CHECK(isnan(result.f));
}

TEST_CASE("ray_scale()", "[mathutil]")
{
    Ufvec vec1, vec2, result, expected;
    vec1.f = {-0.5f, -1.f, 0.3};
    vec2.f = {-0.25f, -1.83f, 2.032f};
    expected = {0xbf966666, 0x3f9ed918, 0xc08c0b78};
    ray_scale(-2.7f, &vec1.f, &vec2.f, &result.f);
    check_vec3f(&result.f, &expected.f);
}

TEST_CASE("vec_set_len()", "[mathutil]")
{
    Ufvec vec1, result, expected;
    vec1.f = {-0.5f, -1.f, 0.3};
    vec_set_len(250.f, &vec1.f, &result.f);
    expected = {0xc2d7f795, 0xc357f795, 0x4281948d};
    check_vec3f(&result.f, &expected.f);
}

TEST_CASE("vec_normalize_len()", "[mathutil]")
{
    Ufvec vec1, expected;
    Uf expected_len;
    expected_len.u = 0x3f942bb5;
    vec1.f = {-0.5f, -1.f, 0.3};
    expected = {0xbedd267c, 0xbf5d267c, 0x3e84b0b1};
    f32 len = vec_normalize_len(&vec1.f);
    check_vec3f(&vec1.f, &expected.f);
    CHECK(len == Approx(expected_len.f));
}

TEST_CASE("mtxa_from_identity()", "[mathutil]")
{
    load_dummy_mtxa();
    mtxa_from_identity();
    Ufmtx m = {
        0x3f800000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x3f800000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x3f800000, 0x00000000
    };
    check_mtxa(m);
}

TEST_CASE("mtx_from_identity()", "[mathutil]")
{
    Ufmtx result;
    mtx_from_identity(&result.f);
    Ufmtx expected = {
        0x3f800000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x3f800000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x3f800000, 0x00000000,
    };
    check_mtx(&result.f, &expected.f);
}

TEST_CASE("mtxa_sq_from_identity()", "[mathutil]")
{
    load_dummy_mtxa();
    mtxa_sq_from_identity();
    Ufmtx result = {
        0x3f800000, 0x00000000, 0x00000000, 0x41cac4b1,
        0x00000000, 0x3f800000, 0x00000000, 0xbf0b1162,
        0x00000000, 0x00000000, 0x3f800000, 0xc22c6076,
    };
    check_mtxa(result);
}

TEST_CASE("mtxa_from_translate_xyz*()", "[mathutil]")
{
    Ufmtx expected;
    Ufvec vec1;
    vec1.f = {-0.5f, -1.f, 0.3};
    load_dummy_mtxa();
    mtxa_from_translate(&vec1.f);
    expected = {
        0x3f800000, 0x00000000, 0x00000000, 0xbf000000,
        0x00000000, 0x3f800000, 0x00000000, 0xbf800000,
        0x00000000, 0x00000000, 0x3f800000, 0x3e99999a,
    };
    check_mtxa(expected);

    load_dummy_mtxa();
    mtxa_from_translate_xyz(vec1.f.x, vec1.f.y, vec1.f.z);
    expected = {
        0x3f800000, 0x00000000, 0x00000000, 0xbf000000,
        0x00000000, 0x3f800000, 0x00000000, 0xbf800000,
        0x00000000, 0x00000000, 0x3f800000, 0x3e99999a,
    };
    check_mtxa(expected);
}

TEST_CASE("mtxa_from_rotate_x()", "[mathutil]")
{
    load_dummy_mtxa();
    mtxa_from_rotate_x(0x38a5);
    Ufmtx m = {
        0x3f800000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x3e37dc4e, 0xbf7bd6f3, 0x00000000,
        0x00000000, 0x3f7bd6f3, 0x3e37dc4e, 0x00000000,
    };
    check_mtxa(m);
}

TEST_CASE("mtxa_from_rotate_y()", "[mathutil]")
{
    load_dummy_mtxa();
    mtxa_from_rotate_y(0x38a5);
    Ufmtx m = {
        0x3e37dc4e, 0x00000000, 0x3f7bd6f3, 0x00000000,
        0x00000000, 0x3f800000, 0x00000000, 0x00000000,
        0xbf7bd6f3, 0x00000000, 0x3e37dc4e, 0x00000000,
    };
    check_mtxa(m);
}

TEST_CASE("mtxa_from_rotate_z()", "[mathutil]")
{
    load_dummy_mtxa();
    mtxa_from_rotate_z(0x38a5);
    Ufmtx m = {
        0x3e37dc4e, 0xbf7bd6f3, 0x00000000, 0x00000000,
        0x3f7bd6f3, 0x3e37dc4e, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x3f800000, 0x00000000,
    };
    check_mtxa(m);
}

TEST_CASE("mtxa_from_mtxb_translate_xyz*()", "[mathutil]")
{
    Ufvec vec1;
    Ufmtx result;
    vec1.f = {-0.5f, -1.f, 0.3};
    load_dummy_mtxa();
    load_dummy_mtxb();
    mtxa_from_mtxb_translate(&vec1.f);
    result = {
        0x3eb22ba4, 0x3f182165, 0xbf399fa9, 0x415780ad,
        0x3da5dd28, 0xbf4a230c, 0xbf1bb6bf, 0xc0c1d985,
        0xbf6f1a37, 0x3e1c9d17, 0xbea55310, 0x3e4cfe5b,
    };
    check_mtxa(result);

    load_dummy_mtxa();
    load_dummy_mtxb();
    mtxa_from_mtxb_translate_xyz(vec1.f.x, vec1.f.y, vec1.f.z);
    result = {
        0x3eb22ba4, 0x3f182165, 0xbf399fa9, 0x415780ad,
        0x3da5dd28, 0xbf4a230c, 0xbf1bb6bf, 0xc0c1d985,
        0xbf6f1a37, 0x3e1c9d17, 0xbea55310, 0x3e4cfe5b,
    };
    check_mtxa(result);
}

TEST_CASE("mtxa_normalize_basis()", "[mathlib]")
{
    load_dummy_mtxa();
    mtxa_scale_xyz(0.3f, 24.3f, -0.52f);
    mtxa_normalize_basis();
    Ufmtx expected = {
        0x3cfbd05e, 0xbf377c09, 0xbf3258ba, 0x41cac4b1,
        0xbe163ac7, 0x3f2fab8f, 0xbf36635d, 0xbf0b1162,
        0xbf7d1b93, 0xbdfe2bdb, 0x3dac2691, 0xc22c6076,
    };
    check_mtxa(expected);
}

TEST_CASE("mtx stack", "[mathlib]")
{
    Ufmtx first_pop, first_peek, second_pop, third_pop;

    mtxa_from_identity();
    mtxa_push();
    load_dummy_mtxa();
    mtxa_push();
    load_dummy_mtxb();
    mtxa_from_mtxb();
    mtxa_push();
    mtxa_from_identity();

    first_pop = {
        0x3eb22ba4, 0x3f182165, 0xbf399fa9, 0x41674670,
        0x3da5dd28, 0xbf4a230c, 0xbf1bb6bf, 0xc0d3fb52,
        0xbf6f1a37, 0x3e1c9d17, 0xbea55310, 0xbc8b3933,
    };
    first_peek = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41cac4b1,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf0b1162,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22c6076,
    };
    second_pop = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41cac4b1,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf0b1162,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22c6076,
    };
    third_pop = {
        0x3f800000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x3f800000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x3f800000, 0x00000000,
    };

    mtxa_pop();
    check_mtxa(first_pop);
    mtxa_peek();
    mtxa_peek();
    check_mtxa(first_peek);
    mtxa_pop();
    check_mtxa(second_pop);
    mtxa_pop();
    check_mtxa(third_pop);
}

TEST_CASE("mtx square copying", "[mathutil]")
{
    Ufmtx expected;

    Ufmtx mtx = gen_dummy_ufmtx1();
    mtxa_from_identity();
    mtxa_sq_from_mtx(&mtx.f);
    expected = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x00000000,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0x00000000,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0x00000000,
    };
    check_mtxa(expected);

    mtx = gen_dummy_ufmtx2();
    load_dummy_mtxa();
    mtxa_sq_to_mtx(&mtx.f);
    expected = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41674670,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xc0d3fb52,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xbc8b3933,
    };
    check_mtx(&mtx.f, &expected.f);
}

TEST_CASE("mtx copying", "[mathutil]")
{
    // mtxa_to_mtx() identity:
    Ufmtx ufmtx1 = {
        0x3f800000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x3f800000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x3f800000, 0x00000000,
    };
    // mtxa_from_mtx() dummy:
    Ufmtx ufmtx2 = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41cac4b1,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf0b1162,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22c6076,
    };
    // mtxa_from_mtxb() dummy:
    Ufmtx ufmtx3 = {
        0x3eb22ba4, 0x3f182165, 0xbf399fa9, 0x41674670,
        0x3da5dd28, 0xbf4a230c, 0xbf1bb6bf, 0xc0d3fb52,
        0xbf6f1a37, 0x3e1c9d17, 0xbea55310, 0xbc8b3933,
    };
        // mtxa_to_mtxb() dummy:
    Ufmtx ufmtx4 = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41cac4b1,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf0b1162,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22c6076,
    };
    // mtx_copy() mtxb:
    Ufmtx ufmtx5 = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41cac4b1,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf0b1162,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22c6076,
    };

    Ufmtx mtx1, mtx2;
    mtxa_from_identity();
    mtxa_to_mtx(&mtx1.f);
    mtxa_to_mtx(&mtx2.f);
    check_mtx(&mtx1.f, &ufmtx1.f);

    mtx1 = gen_dummy_ufmtx1();
    mtxa_from_mtx(&mtx1.f);
    check_mtxa(ufmtx2);

    load_dummy_mtxb();
    mtxa_from_mtxb();
    check_mtxa(ufmtx3);

    load_dummy_mtxa();
    mtxa_to_mtxb();
    check_mtxb(ufmtx4);

    mtx_copy(&mtx1.f, &mtx2.f);
    check_mtx(&mtx2.f, &ufmtx5.f);
}

TEST_CASE("mtxa_invert()", "[mathutil]")
{
    load_dummy_mtxa();
    mtxa_invert();
    Ufmtx mtx = {
        0x3cfbd06a, 0xbe163ac9, 0xbf7d1b94, 0xc22ddd52,
        0xbf377c0a, 0x3f2fab91, 0xbdfe2bdb, 0x41530df8,
        0x3f3258bb, 0x3f36635e, 0xbdac2691, 0xc1a7251a,
    };
    check_mtxa(mtx);
}

TEST_CASE("mtxa_transpose()", "[mathutil]")
{
    load_dummy_mtxa();
    mtxa_transpose();
    Ufmtx mtx = {
        0x3cfbd060, 0xbe163ac8, 0xbf7d1b94, 0xc22ddd52,
        0xbf377c09, 0x3f2fab90, 0xbdfe2bdc, 0x41530df5,
        0x3f3258ba, 0x3f36635c, 0xbdac2690, 0xc1a72519,
    };
    check_mtxa(mtx);
}

TEST_CASE("math mtx mult", "[mathutil]")
{
    Ufmtx result;

    load_dummy_mtxa();
    Ufmtx mtx = gen_dummy_ufmtx2();
    mtxa_mult_right(&mtx.f);
    result = {
        0xbf32b240, 0x3f30d502, 0x3e413f62, 0x41f43642,
        0xbf293115, 0xbf0521ee, 0xbf0a8358, 0xc0e7186a,
        0xbe8d1b5a, 0xbf009dc0, 0x3f51ccd4, 0xc2623f82,
    };
    check_mtxa(result);

    load_dummy_mtxa();
    mtxa_mult_left(&mtx.f);
    result = {
        0x3f23f231, 0x3e7e5231, 0x3f3a0d3d, 0x4258cc16,
        0x3f383f45, 0xbf063e5b, 0xbee8f5dd, 0x41b08e8a,
        0x3e8944d1, 0x3f507ff8, 0xbf03bb30, 0xc11dba83,
    };
    check_mtxa(result);

    mtx = gen_dummy_ufmtx1();
    load_dummy_mtxb();
    mtxa_from_mtxb_mult_mtx(&mtx.f);
    result = {
        0x3f23f231, 0x3e7e5231, 0x3f3a0d3d, 0x4258cc16,
        0x3f383f45, 0xbf063e5b, 0xbee8f5dd, 0x41b08e8a,
        0x3e8944d1, 0x3f507ff8, 0xbf03bb30, 0xc11dba83,
    };
    check_mtxa(result);

    Ufmtx mtx_left = gen_dummy_ufmtx1();
    Ufmtx mtx_right = gen_dummy_ufmtx2();
    Ufmtx result2;
    mtx_mult(&mtx_left.f, &mtx_right.f, &result2.f);
    result = {
        0xbf32b240, 0x3f30d502, 0x3e413f62, 0x41f43642,
        0xbf293115, 0xbf0521ee, 0xbf0a8358, 0xc0e7186a,
        0xbe8d1b5a, 0xbf009dc0, 0x3f51ccd4, 0xc2623f82,
    };
    check_mtx(&result2.f, &result.f);
}

TEST_CASE("math mtxa tfset", "[mathutil]")
{
    Ufvec vec1;
    Ufmtx expected;

    vec1.f = {-0.5f, -1.f, 0.3};
    load_dummy_mtxa();
    mtxa_translate(&vec1.f);
    expected = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41d20d1f,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf713e30,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22a00fb,
    };
    check_mtxa(expected);

    load_dummy_mtxa();
    mtxa_translate_xyz(vec1.f.x, vec1.f.y, vec1.f.z);
    expected = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41d20d1f,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf713e30,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22a00fb,
    };
    check_mtxa(expected);

    load_dummy_mtxa();
    mtxa_translate_neg(&vec1.f);
    expected = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41c37c43,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbe13924e,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22ebff1,
    };
    check_mtxa(expected);

    load_dummy_mtxa();
    mtxa_translate_neg_xyz(vec1.f.x, vec1.f.y, vec1.f.z);
    expected = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41c37c43,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbe13924e,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22ebff1,
    };
    check_mtxa(expected);
}

TEST_CASE("mtxa_scale_xyz*()", "[mathutil]")
{
    Ufmtx expected;
    load_dummy_mtxa();
    Ufvec vec1;
    vec1.f = {-0.5f, -1.f, 0.3};
    mtxa_scale(&vec1.f);
    expected = {
        0xbc7bd060, 0x3f377c09, 0x3e560413, 0x41cac4b1,
        0x3d963ac8, 0xbf2fab90, 0x3e5adda2, 0xbf0b1162,
        0x3efd1b94, 0x3dfe2bdc, 0xbcce94ad, 0xc22c6076,
    };
    check_mtxa(expected);

    load_dummy_mtxa();
    mtxa_scale_s(7.3f);
    expected = {
        0x3e65c7be, 0xc0a76dfc, 0x40a2bdc4, 0x41cac4b1,
        0xbf8915a4, 0x40a04c8d, 0x40a66dde, 0xbf0b1162,
        0xc0e6f5f7, 0xbf67ee6c, 0xbf1d1664, 0xc22c6076,
    };
    check_mtxa(expected);

    load_dummy_mtxa();
    mtxa_scale_xyz(vec1.f.x, vec1.f.y, vec1.f.z);
    expected = {
        0xbc7bd060, 0x3f377c09, 0x3e560413, 0x41cac4b1,
        0x3d963ac8, 0xbf2fab90, 0x3e5adda2, 0xbf0b1162,
        0x3efd1b94, 0x3dfe2bdc, 0xbcce94ad, 0xc22c6076,
    };
    check_mtxa(expected);
}

TEST_CASE("math_mtxa_rigid_inv_tf*()", "[mathutil]")
{
    Ufvec vec1, dst, expected;
    vec1.f = {-0.5f, -1.f, 0.3};

    // Load dummy rigid transform into Matrix A
    mtxa_from_translate_xyz(0.1f, -4.2f, 7.5f);
    mtxa_rotate_x(0x38a0);
    mtxa_rotate_y(-0x6803);
    mtxa_rotate_z(0xc800);

    mtxa_rigid_inv_tf_point(&vec1.f, &dst.f);
    expected = {0x40bfe8dd, 0xc04cd244, 0x4080ec73};
    check_vec3f(&dst.f, &expected.f);

    mtxa_rigid_inv_tf_point_xyz(vec1.f.x, vec1.f.y, vec1.f.z, &dst.f);
    expected = {0x40bfe8dd, 0xc04cd244, 0x4080ec73};
    check_vec3f(&dst.f, &expected.f);

    mtxa_rigid_inv_tf_tl(&dst.f);
    expected = {0x40bd53f4, 0xc08643e2, 0x4093a727};
    check_vec3f(&dst.f, &expected.f);

    mtxa_rigid_inv_tf_vec(&vec1.f, &dst.f);
    expected = {0x3da53a42, 0x3f7ed5fb, 0xbf15d59d};
    check_vec3f(&dst.f, &expected.f);

    mtxa_rigid_inv_tf_vec_xyz(vec1.f.x, vec1.f.y, vec1.f.z, &dst.f);
    expected = {0x3da53a42, 0x3f7ed5fb, 0xbf15d59d};
    check_vec3f(&dst.f, &expected.f);
}

TEST_CASE("inverse rotation mtx equals inverse mtx", "[mathutil]")
{
    mtxa_from_translate_xyz(0.1f, -4.2f, 7.5f);
    mtxa_rotate_x(0x38a0);
    mtxa_rotate_y(-0x6803);
    mtxa_rotate_z(0xc800);
    mtxa_to_mtxb();
    mtxa_transpose();
    Ufmtx rot_inv, inv;
    mtxa_to_mtx(&rot_inv.f);
    mtxa_from_mtxb();
    mtxa_invert();
    mtxa_to_mtx(&inv.f);
    check_mtx(&rot_inv.f, &inv.f);
}

TEST_CASE("rotate mtx", "[mathutil]")
{
    load_dummy_mtxa();
    mtxa_rotate_x(-0x5c91);
    mtxa_rotate_z(0x20a9);
    mtxa_rotate_y(0x5dff);
    Ufmtx expected = {
        0x3f421ced, 0xbd90ec15, 0x3f25ea63, 0x41cac4b1,
        0x3efe5414, 0xbf14c637, 0xbf250504, 0xbf0b1162,
        0x3ed832b2, 0x3f4f8afa, 0xbecf9ed2, 0xc22c6076,
    };
    check_mtxa(expected);
}

TEST_CASE("quat", "[mathutil]")
{
    Ufvec vec1, vec2, expected_vec;
    Ufquat q1, q2, prod, expected;
    Ufmtx expected_mtx;
    load_dummy_mtxa();
    mtxa_to_quat(&q1.f);
    expected = {0xbea79815, 0x3f28d222, 0x3e646533, 0x3f23907c};
    check_quat(&q1.f, &expected.f);

    load_dummy_mtxb();
    mtxa_from_mtxb();
    mtxa_to_quat(&q2.f);
    quat_mult(&prod.f, &q1.f, &q2.f);
    expected = {0x3ccc2e87, 0x3e9944f4, 0xbf5f1687, 0x3ec68946};
    check_quat(&prod.f, &expected.f);

    mtxa_from_quat(&prod.f);
    expected_mtx = {
        0xbf32b242, 0x3f30d502, 0x3e413f69, 0x00000000,
        0xbf293116, 0xbf0521f0, 0xbf0a8359, 0x00000000,
        0xbe8d1b54, 0xbf009dc2, 0x3f51ccd4, 0x00000000,
    };
    check_mtxa(expected_mtx);

    Ud d, expected_d;
    d.d = quat_to_axis_angle(&q2.f, &vec2.f);
    expected_vec = {0x3f4ef6f8, 0x3e6331e6, 0xbf0b8dff};
    expected_d.u = 0x4005361e21a02bc4;
    check_vec3f(&vec2.f, &expected_vec.f);
    CHECK(d.d == Approx(expected_d.d));
}

TEST_CASE("quat_from_axis_angle()", "[mathutil]")
{
    Ufquat q1, expected;
    Ufvec vec1;
    vec1.f = {-3.5f, -1.007f, 8.3};
    quat_from_axis_angle(&q1.f, &vec1.f, -0x5c91);
    expected = {0x3eb35016, 0x3dce5d0b, 0xbf549d22, 0x3ed7a9dd};
    check_quat(&q1.f, &expected.f); // TODO fix precision issues or ignore
}

TEST_CASE("quat_normalize()", "[mathutil]")
{
    Ufquat q1, expected;
    load_dummy_mtxa();
    mtxa_to_quat(&q1.f);
    q1.f.x *= 3.2f;
    q1.f.y *= 3.2f;
    q1.f.z *= 3.2f;
    q1.f.w *= 3.2f;
    quat_normalize(&q1.f);
    expected = {0xbea79814, 0x3f28d221, 0x3e646532, 0x3f23907b};
    check_quat(&q1.f, &expected.f);
}

TEST_CASE("quat_from_dirs()", "[mathutil]")
{
    Ufquat q1, expected;
    Ufvec vec1, vec2;
    vec1.f = {-0.5f, -1.f, 0.3};
    vec2.f = {-0.25f, -1.83f, 2.032f};
    vec_normalize_len(&vec1.f);
    vec_normalize_len(&vec2.f);
    quat_from_dirs(&q1.f, &vec1.f, &vec2.f);
    expected = {0xbe7b50e0, 0x3e1f7755, 0x3de1634a, 0x3f735258};
    check_quat(&q1.f, &expected.f);

    // TODO SMB's function has peculiar behavior when you pass it non-normalized vectors which we don't account for here.
    // Fix this and test it here
}

TEST_CASE("quat_slerp()", "[mathutil]")
{
    Ufquat q1, q2, prod, expected;
    load_dummy_mtxa();
    load_dummy_mtxb();
    mtxa_to_quat(&q1.f);
    mtxa_from_mtxb();
    mtxa_to_quat(&q2.f);
    quat_slerp(0.75f, &prod.f, &q1.f, &q2.f);
    expected = {0xbf53e5cb, 0x3d4014b8, 0x3f0f193c, 0x3c700f30};
    check_quat(&prod.f, &expected.f);
}

TEST_CASE("ray_to_euler*()", "[mathutil]")
{
    Ufvec vec1, vec2, vec3;
    Vec3s rot = {0x4242, 0x4242, 0x4242};

    vec1.f = {-0.5f, -1.f, 0.3};
    vec2.f = {-0.25f, -1.83f, 2.032f};
    vec3.f = {0.25f, 1.83f, -2.032f};
    ray_to_euler(&vec1.f, &vec2.f, &rot);
    Vec3s expected = {-4619, -31273, 0};
    CHECK(abs(rot.x - expected.x) <= 1);
    CHECK(abs(rot.y - expected.y) <= 1);
    CHECK(abs(rot.z - expected.z) <= 1);

    s16 x, y;
    ray_to_euler_xy(&vec1.f, &vec2.f, &x, &y);
    s16 expected_x = -4619, expected_y = -31273;
    CHECK(abs(x - expected_x) <= 1);
    CHECK(abs(y - expected_y) <= 1);
}

TEST_CASE("vec_to_euler*()", "[mathutil]")
{
    Ufvec vec1;
    Vec3s rot = {0x4242, 0x4242, 0x4242};
    vec1.f = {-0.5f, -1.f, 0.3};

    vec_to_euler(&vec1.f, &rot);
    Vec3s expected = {-10878, 22021, 0};
    CHECK(abs(rot.x - expected.x) <= 1);
    CHECK(abs(rot.y - expected.y) <= 1);
    CHECK(abs(rot.z - expected.z) <= 1);

    s16 x, y;
    vec_to_euler_xy(&vec1.f, &x, &y);
    s16 expected_x = -10878, expected_y = 22021;
    CHECK(abs(x - expected_x) <= 1);
    CHECK(abs(y - expected_y) <= 1);
}

TEST_CASE("mtxa_to_euler*()", "[mathutil]")
{
    load_dummy_mtxa();
    s16 x, y, z;
    mtxa_to_euler_yxz(&y, &x, &z);
    s16 expected_x = -8271, expected_y = 17636, expected_z = -2197;
    CHECK(abs(x - expected_x) <= 1);
    CHECK(abs(y - expected_y) <= 1);
    CHECK(abs(z - expected_z) <= 1);

    Vec3s rot = {0x4242, 0x4242, 0x4242};
    mtxa_to_euler(&rot);
    Vec3s expected = {-8271, 17636, -2197};
    CHECK(abs(rot.x - expected.x) <= 1);
    CHECK(abs(rot.y - expected.y) <= 1);
    CHECK(abs(rot.z - expected.z) <= 1);
}
