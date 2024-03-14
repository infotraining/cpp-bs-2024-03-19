#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace MyMath
{
    // TODO: write Vector2D class

} // namespace MyMath

using namespace MyMath;
using Catch::Approx;

TEST_CASE("Vector2D")
{
    // uncomment lines below
    //

    // SECTION("default constructor")
    // {
    //     Vector2D vec;

    //     REQUIRE(vec.x() == Approx(0.0));
    //     REQUIRE(vec.y() == Approx(0.0));
    // }

    // SECTION("constructor")
    // {
    //     const Vector2D vec{1.0, 2.0};

    //     REQUIRE(vec.x() == Approx(1.0));
    //     REQUIRE(vec.y() == Approx(2.0));
    // }

    // SECTION("length")
    // {
    //     Vector2D vec{1.0, 1.0};

    //     REQUIRE(vec.length() == Approx(1.41).margin(0.01));
    // }

    // SECTION("unit vector x - static function")
    // {
    //     Vector2D vx = Vector2D::unit_x();

    //     REQUIRE(vx.x() == Approx(1.0));
    //     REQUIRE(vx.y() == Approx(0.0));
    // }

    // SECTION("unit vector y - static function")
    // {
    //     Vector2D vy = Vector2D::unit_y();

    //     REQUIRE(vy.x() == Approx(0.0));
    //     REQUIRE(vy.y() == Approx(1.0));
    // }
}

TEST_CASE("Vector2D - operators")
{
    // uncomment lines below
    //

    // const Vector2D vec1{ 1.0, 2.0 };
    // const Vector2D vec2{ 2.0, 0.5 };

    // SECTION("equality")
    // {
    // 	REQUIRE(vec1 == Vector2D{ 1.0, 2.0 });
    // }

    // SECTION("non-equality")
    // {
    // 	REQUIRE(vec1 != vec2);
    // }

    // SECTION("subtraction")
    // {
    // 	auto vec = vec1 - vec2;

    // 	REQUIRE(vec == Vector2D{ -1.0, 1.5 });
    // }

    // SECTION("negate")
    // {
    // 	auto vec = -vec1;

    // 	REQUIRE(vec == Vector2D{ -1.0, -2.0 });
    // }

    // SECTION("multiplication")
    // {
    // 	double scalar = vec1 * vec2;

    // 	REQUIRE(scalar == Approx(3.0));
    // }

    // SECTION("multiplication by double")
    // {
    // 	SECTION("a * Vector2D")
    // 	{
    // 		Vector2D vec = 2.0 * vec1;

    // 		REQUIRE(vec == Vector2D{ 2.0, 4.0 });
    // 	}

    // 	SECTION("Vector2D * a")
    // 	{
    // 		Vector2D vec = vec1 * 2.0;

    // 		REQUIRE(vec == Vector2D{ 2.0, 4.0 });
    // 	}
    // }

    // SECTION("operator <<")
    // {
    // 	cout << "vec1: " << vec1 << endl;

    // 	stringstream ss;

    // 	ss << vec1;

    // 	REQUIRE(ss.str() == "Vector2D(1.0, 2.0)");
    // }
}
