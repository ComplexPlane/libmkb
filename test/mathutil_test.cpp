#include <catch.hpp>

#include "mathutil.h"

using namespace mkb2;

// Useful for comparing exact floating-point values with the original game
union uf32
{
    u32 u;
    f32 f;
};
union uf64
{
    u64 u;
    f64 f;
};
union ufarr32
{
    u32 u[2];
    f32 f[2];
};

TEST_CASE("Square-root functions work", "[mathutil]")
{
    uf64 a, b;
    uf32 c, d, e, f;

    // Positive square-root
    a.u = 0x404d9f62d83c6c98;
    c.u = 0x40f64e91;
    CHECK(math_sqrt(a.f) == Approx(c.f));

    // Negative square-root
    CHECK(math_sqrt(-523.3234) == Approx(0.f));

    // Positive values are correct
    a.u = 0x404d9f62d83c6c98;
    c.u = 0x3e05098b;
    CHECK(math_rsqrt(a.f) == Approx(c.f));

    // Negative values return INFINITY
    a.u = 0xc0469e2339c0ebee;
    c.u = 0x7f800000;
    CHECK(math_rsqrt(a.f) == Approx(c.f));

    // Positive input to math_sqrt_rsqrt
    a.u = 0x404d9f62d83c6c98;
    c.u = 0x40f64e91;
    d.u = 0x3e05098b;
    f.f = math_sqrt_rsqrt(a.f, &e.f);
    CHECK(c.f == Approx(e.f));
    CHECK(d.f == Approx(f.f));

    // Negative input to math_sqrt_rsqrt
    a.u = 0xc0469e2339c0ebee;
    c.u = 0x0;
    d.u = 0x7f800000;
    f.f = math_sqrt_rsqrt(a.f, &e.f);
    CHECK(c.f == Approx(e.f));
    CHECK(d.f == Approx(f.f));
}

TEST_CASE("Trig functions work", "[mathutil]")
{
    uf32 a, b;
    s16 angle;
    ufarr32 arr1, arr2;

    // math_sin
    angle = 0x35a2;
    a.u = 0x3f77c1e1;
    CHECK(math_sin(angle) == Approx(a.f));

    // math_sin_cos_v
    angle = 0x35a2;
    arr2.u[0] = 0x3f77c1e1;
    arr2.u[1] = 0x3e80e06e;
    math_sin_cos_v(angle, arr1.f);
    CHECK(arr1.f[0] == Approx(arr2.f[0]));
    CHECK(arr1.f[0] == Approx(arr2.f[0]));
}