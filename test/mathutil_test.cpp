#include <catch.hpp>

#include "mathutil.h"
#include "global_state.h"

using namespace mkb2;

// Useful for comparing exact floating-point values with the original game

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

void check_mtxa(const Ufmtx &ufmtx)
{
    CHECK(ufmtx.f[0][0] == Approx(gs->mtxa_raw[0][0]));
    CHECK(ufmtx.f[0][1] == Approx(gs->mtxa_raw[0][1]));
    CHECK(ufmtx.f[0][2] == Approx(gs->mtxa_raw[0][2]));
    CHECK(ufmtx.f[0][3] == Approx(gs->mtxa_raw[0][3]));

    CHECK(ufmtx.f[1][0] == Approx(gs->mtxa_raw[1][0]));
    CHECK(ufmtx.f[1][1] == Approx(gs->mtxa_raw[1][1]));
    CHECK(ufmtx.f[1][2] == Approx(gs->mtxa_raw[1][2]));
    CHECK(ufmtx.f[1][3] == Approx(gs->mtxa_raw[1][3]));

    CHECK(ufmtx.f[2][0] == Approx(gs->mtxa_raw[2][0]));
    CHECK(ufmtx.f[2][1] == Approx(gs->mtxa_raw[2][1]));
    CHECK(ufmtx.f[2][2] == Approx(gs->mtxa_raw[2][2]));
    CHECK(ufmtx.f[2][3] == Approx(gs->mtxa_raw[2][3]));
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

TEST_CASE("math_mtxa_from_identity()", "[mathutil]")
{
    math_mtxa_from_identity();
    Ufmtx m = {
        0x3f800000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x3f800000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x3f800000, 0x00000000
    };
    check_mtxa(m);
}