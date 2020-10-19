#include <catch.hpp>

#include "mathutil.h"

using namespace mkb2;

TEST_CASE("Scalar math functions work", "[mathutil]")
{
    // TODO pull values from actual game to test accuracy

    CHECK(math_sqrt(64.) == Approx(8.f));
    CHECK(math_sqrt(0.) == Approx(0.f));
    CHECK(math_sqrt(-5.2) == Approx(0.f));

    CHECK(math_rsqrt(4.) == Approx(0.5f));
    CHECK(math_rsqrt(0.) == Approx(INFINITY));
    CHECK(math_rsqrt(-8.234) == Approx(INFINITY));
}
