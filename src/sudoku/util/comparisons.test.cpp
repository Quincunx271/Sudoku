#include <sudoku/util/comparisons.hpp>

#include <functional>

#include <catch2/catch.hpp>

using sudoku::eq;
using sudoku::eq_t;
using sudoku::ge;
using sudoku::ge_t;
using sudoku::gt;
using sudoku::gt_t;
using sudoku::le;
using sudoku::le_t;
using sudoku::lt;
using sudoku::lt_t;
using sudoku::ne;
using sudoku::ne_t;

TEST_CASE("lt works")
{
	STATIC_REQUIRE(lt(1, 2));
	STATIC_REQUIRE_FALSE(lt(2, 2));
	STATIC_REQUIRE_FALSE(lt(3, 2));
	STATIC_REQUIRE(lt(2)(1));
	STATIC_REQUIRE_FALSE(lt(2)(2));
	STATIC_REQUIRE_FALSE(lt(2)(3));
}

TEST_CASE("le works")
{
	STATIC_REQUIRE(le(1, 2));
	STATIC_REQUIRE(le(2, 2));
	STATIC_REQUIRE_FALSE(le(3, 2));
	STATIC_REQUIRE(le(2)(1));
	STATIC_REQUIRE(le(2)(2));
	STATIC_REQUIRE_FALSE(le(2)(3));
}

TEST_CASE("gt works")
{
	STATIC_REQUIRE_FALSE(gt(1, 2));
	STATIC_REQUIRE_FALSE(gt(2, 2));
	STATIC_REQUIRE(gt(3, 2));
	STATIC_REQUIRE_FALSE(gt(2)(1));
	STATIC_REQUIRE_FALSE(gt(2)(2));
	STATIC_REQUIRE(gt(2)(3));
}

TEST_CASE("ge works")
{
	STATIC_REQUIRE_FALSE(ge(1, 2));
	STATIC_REQUIRE(ge(2, 2));
	STATIC_REQUIRE(ge(3, 2));
	STATIC_REQUIRE_FALSE(ge(2)(1));
	STATIC_REQUIRE(ge(2)(2));
	STATIC_REQUIRE(ge(2)(3));
}

TEST_CASE("eq works")
{
	STATIC_REQUIRE_FALSE(eq(1, 2));
	STATIC_REQUIRE(eq(2, 2));
	STATIC_REQUIRE_FALSE(eq(3, 2));
	STATIC_REQUIRE_FALSE(eq(2)(1));
	STATIC_REQUIRE(eq(2)(2));
	STATIC_REQUIRE_FALSE(eq(2)(3));
}

TEST_CASE("ne works")
{
	STATIC_REQUIRE(ne(1, 2));
	STATIC_REQUIRE_FALSE(ne(2, 2));
	STATIC_REQUIRE(ne(3, 2));
	STATIC_REQUIRE(ne(2)(1));
	STATIC_REQUIRE_FALSE(ne(2)(2));
	STATIC_REQUIRE(ne(2)(3));
}

template <typename T>
constexpr bool is_transparent_v = requires
{
	typename T::is_transparent;
};

TEST_CASE("All are transparent")
{
	static_assert(is_transparent_v<std::less<>>,
		"This test assumes that the existance of an ::is_transparent member type is what indicates "
		"a transparent comparator.");
	STATIC_REQUIRE(is_transparent_v<lt_t>);
	STATIC_REQUIRE(is_transparent_v<le_t>);
	STATIC_REQUIRE(is_transparent_v<gt_t>);
	STATIC_REQUIRE(is_transparent_v<ge_t>);
	STATIC_REQUIRE(is_transparent_v<eq_t>);
	STATIC_REQUIRE(is_transparent_v<ne_t>);
}
