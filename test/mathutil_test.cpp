#include <catch.hpp>

#include "mathutil.h"
#include "global_state.h"

using namespace mkb2;

// Useful for comparing exact floating-point values with the original game

union Uf
{
    u32 u;
    f32 f;
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
    if (isnan(expected->x))
    {
        CHECK(isnan(result->x));
    }
    else
    {
        CHECK(result->x == Approx(expected->x));
    }

    if (isnan(expected->y))
    {
        CHECK(isnan(result->y));
    }
    else
    {
        CHECK(result->y == Approx(expected->y));
    }

    if (isnan(expected->z))
    {
        CHECK(isnan(result->z));
    }
    else
    {
        CHECK(result->z == Approx(expected->z));
    }
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

Ufmtx gen_dummy_ufmtx()
{
    return {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41cac4b1,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf0b1162,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22c6076,
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

TEST_CASE("math_vec_dot_normalized_*()", "[mathutil]")
{
    Ufvec vec1, vec2, vec3, vec4, vec5;
    vec1.f = {-0.5f, -1.f, 0.3};
    vec2.f = {-0.25f, -1.83f, 2.032f};
    vec3.f = {0.25f, 1.83f, -2.032f};
    vec4.f = {0.f, 0.f, 0.f};
    vec5.f = {INFINITY, 0.5, -INFINITY};

    Uf result, expected;

    result.f = math_vec_dot_normalized_safe(&vec1.f, &vec2.f);
    expected.u = 0x3f4e8add;
    CHECK(result.f == Approx(expected.f));

    result.f = math_vec_dot_normalized_safe(&vec1.f, &vec3.f);
    expected.u = 0xbf4e8add;
    CHECK(result.f == Approx(expected.f));

    result.f = math_vec_dot_normalized_safe(&vec1.f, &vec4.f);
    expected.u = 0x00000000;
    CHECK(result.f == Approx(expected.f));

    result.f = math_vec_dot_normalized_safe(&vec1.f, &vec5.f);
    expected.u = 0x7f800000;
    CHECK(result.f == Approx(expected.f));

    result.f = math_vec_dot_normalized_safe(&vec4.f, &vec4.f);
    expected.u = 0x00000000;
    CHECK(result.f == Approx(expected.f));

    result.f = math_vec_dot_normalized(&vec1.f, &vec2.f);
    expected.u = 0x3f4e8add;
    CHECK(result.f == Approx(expected.f));

    result.f = math_vec_dot_normalized(&vec1.f, &vec3.f);
    expected.u = 0xbf4e8add;
    CHECK(result.f == Approx(expected.f));

    result.f = math_vec_dot_normalized(&vec1.f, &vec4.f);
    expected.u = 0xffc00000;
    CHECK(isnan(result.f));

    result.f = math_vec_dot_normalized(&vec1.f, &vec5.f);
    expected.u = 0xffc00000;
    CHECK(isnan(result.f));

    result.f = math_vec_dot_normalized(&vec4.f, &vec4.f);
    expected.u = 0xffc00000;
    CHECK(isnan(result.f));
}

TEST_CASE("math_ray_scale()", "[mathutil]")
{
    Ufvec vec1, vec2, result, expected;
    vec1.f = {-0.5f, -1.f, 0.3};
    vec2.f = {-0.25f, -1.83f, 2.032f};
    expected = {0xbf966666, 0x3f9ed918, 0xc08c0b78};
    math_ray_scale(-2.7f, &vec1.f, &vec2.f, &result.f);
    check_vec3f(&result.f, &expected.f);
}

TEST_CASE("math_vec_normalize_len()", "[mathutil]")
{
    Ufvec vec1, expected;
    Uf expected_len;
    expected_len.u = 0x3f942bb5;
    vec1.f = {-0.5f, -1.f, 0.3};
    expected = {0xbedd267c, 0xbf5d267c, 0x3e84b0b1};
    f32 len = math_vec_normalize_len(&vec1.f);
    check_vec3f(&vec1.f, &expected.f);
    CHECK(len == Approx(expected_len.f));
}

TEST_CASE("math_mtxa_from_identity()", "[mathutil]")
{
    load_dummy_mtxa();
    math_mtxa_from_identity();
    Ufmtx m = {
        0x3f800000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x3f800000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x3f800000, 0x00000000
    };
    check_mtxa(m);
}

TEST_CASE("math_mtx_from_identity()", "[mathutil]")
{
    Ufmtx result;
    math_mtx_from_identity(&result.f);
    Ufmtx expected = {
        0x3f800000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x3f800000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x3f800000, 0x00000000,
    };
    check_mtx(&result.f, &expected.f);
}

TEST_CASE("math_mtxa_sq_from_identity()", "[mathutil]")
{
    load_dummy_mtxa();
    math_mtxa_sq_from_identity();
    Ufmtx result = {
        0x3f800000, 0x00000000, 0x00000000, 0x41cac4b1,
        0x00000000, 0x3f800000, 0x00000000, 0xbf0b1162,
        0x00000000, 0x00000000, 0x3f800000, 0xc22c6076,
    };
    check_mtxa(result);
}

TEST_CASE("math_mtxa_from_rotate_x()", "[mathutil]")
{
    load_dummy_mtxa();
    math_mtxa_from_rotate_x(0x38a5);
    Ufmtx m = {
        0x3f800000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x3e37dc4e, 0xbf7bd6f3, 0x00000000,
        0x00000000, 0x3f7bd6f3, 0x3e37dc4e, 0x00000000,
    };
    check_mtxa(m);
}

TEST_CASE("math_mtxa_from_rotate_y()", "[mathutil]")
{
    load_dummy_mtxa();
    math_mtxa_from_rotate_y(0x38a5);
    Ufmtx m = {
        0x3e37dc4e, 0x00000000, 0x3f7bd6f3, 0x00000000,
        0x00000000, 0x3f800000, 0x00000000, 0x00000000,
        0xbf7bd6f3, 0x00000000, 0x3e37dc4e, 0x00000000,
    };
    check_mtxa(m);
}

TEST_CASE("math_mtxa_from_rotate_z()", "[mathutil]")
{
    load_dummy_mtxa();
    math_mtxa_from_rotate_z(0x38a5);
    Ufmtx m = {
        0x3e37dc4e, 0xbf7bd6f3, 0x00000000, 0x00000000,
        0x3f7bd6f3, 0x3e37dc4e, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x3f800000, 0x00000000,
    };
    check_mtxa(m);
}

TEST_CASE("math_mtxa_from_mtxb_tfset_point*()", "[mathutil]")
{
    Ufvec vec1;
    Ufmtx result;
    vec1.f = {-0.5f, -1.f, 0.3};
    load_dummy_mtxa();
    load_dummy_mtxb();
    math_mtxa_from_mtxb_tfset_point_v(&vec1.f);
    result = {
        0x3eb22ba4, 0x3f182165, 0xbf399fa9, 0x415780ad,
        0x3da5dd28, 0xbf4a230c, 0xbf1bb6bf, 0xc0c1d985,
        0xbf6f1a37, 0x3e1c9d17, 0xbea55310, 0x3e4cfe5b,
    };
    check_mtxa(result);

    load_dummy_mtxa();
    load_dummy_mtxb();
    math_mtxa_from_mtxb_tfset_point(vec1.f.x, vec1.f.y, vec1.f.z);
    result = {
        0x3eb22ba4, 0x3f182165, 0xbf399fa9, 0x415780ad,
        0x3da5dd28, 0xbf4a230c, 0xbf1bb6bf, 0xc0c1d985,
        0xbf6f1a37, 0x3e1c9d17, 0xbea55310, 0x3e4cfe5b,
    };
    check_mtxa(result);
}

TEST_CASE("math_mtxa_sq_normalize()", "[mathlib]")
{
    // TODO
//    Ufmtx ufmtx = {
//        0x3cfbd060, 0xbf377c0a, 0x3f3258ba, 0x41cac4b1,
//        0xbe163ac7, 0x3f2fab90, 0x3f36635d, 0xbf0b1162,
//        0xbf7d1b94, 0xbdfe2bdd, 0xbdac2690, 0xc22c6076,
//    };
//    load_dummy_mtxa();
//    math_mtxa_mult_scale_s(100.f);
//    math_mtxa_sq_normalize();
//    check_mtxa(ufmtx);
}

TEST_CASE("mtx stack", "[mathlib]")
{
    Ufmtx first_pop, first_peek, second_pop, third_pop;

    math_mtxa_from_identity();
    math_mtxa_push();
    load_dummy_mtxa();
    math_mtxa_push();
    load_dummy_mtxb();
    math_mtxa_from_mtxb();
    math_mtxa_push();
    math_mtxa_from_identity();

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

    math_mtxa_pop();
    check_mtxa(first_pop);
    math_mtxa_peek();
    math_mtxa_peek();
    check_mtxa(first_peek);
    math_mtxa_pop();
    check_mtxa(second_pop);
    math_mtxa_pop();
    check_mtxa(third_pop);
}

TEST_CASE("mtx copying", "[mathutil]")
{
    // math_mtxa_to_mtx() identity:
    Ufmtx ufmtx1 = {
        0x3f800000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x3f800000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x3f800000, 0x00000000,
    };
    // math_mtxa_from_mtx() dummy:
    Ufmtx ufmtx2 = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41cac4b1,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf0b1162,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22c6076,
    };
    // math_mtxa_from_mtxb() dummy:
    Ufmtx ufmtx3 = {
        0x3eb22ba4, 0x3f182165, 0xbf399fa9, 0x41674670,
        0x3da5dd28, 0xbf4a230c, 0xbf1bb6bf, 0xc0d3fb52,
        0xbf6f1a37, 0x3e1c9d17, 0xbea55310, 0xbc8b3933,
    };
        // math_mtxa_to_mtxb() dummy:
    Ufmtx ufmtx4 = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41cac4b1,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf0b1162,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22c6076,
    };
    // math_mtx_copy() mtxb:
    Ufmtx ufmtx5 = {
        0x3cfbd060, 0xbf377c09, 0x3f3258ba, 0x41cac4b1,
        0xbe163ac8, 0x3f2fab90, 0x3f36635c, 0xbf0b1162,
        0xbf7d1b94, 0xbdfe2bdc, 0xbdac2690, 0xc22c6076,
    };

    Ufmtx mtx1, mtx2;
    math_mtxa_from_identity();
    math_mtxa_to_mtx(&mtx1.f);
    math_mtxa_to_mtx(&mtx2.f);
    check_mtx(&mtx1.f, &ufmtx1.f);

    mtx1 = gen_dummy_ufmtx();
    math_mtxa_from_mtx(&mtx1.f);
    check_mtxa(ufmtx2);

    load_dummy_mtxb();
    math_mtxa_from_mtxb();
    check_mtxa(ufmtx3);

    load_dummy_mtxa();
    math_mtxa_to_mtxb();
    check_mtxb(ufmtx4);

    math_mtx_copy(&mtx1.f, &mtx2.f);
    check_mtx(&mtx2.f, &ufmtx5.f);
}