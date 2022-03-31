#include <sudoku/util/comparisons.hpp>

#include <functional>
#include <sudoku/util/util.hpp>

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

TEST_CASE("Captured is default constructible when empty")
{
	using sudoku::val;
	STATIC_REQUIRE(std::is_default_constructible_v<std::remove_cvref_t<decltype(lt(val<1>))>>);
	STATIC_REQUIRE(std::is_default_constructible_v<std::remove_cvref_t<decltype(le(val<1>))>>);
	STATIC_REQUIRE(std::is_default_constructible_v<std::remove_cvref_t<decltype(gt(val<1>))>>);
	STATIC_REQUIRE(std::is_default_constructible_v<std::remove_cvref_t<decltype(ge(val<1>))>>);
	STATIC_REQUIRE(std::is_default_constructible_v<std::remove_cvref_t<decltype(eq(val<1>))>>);
	STATIC_REQUIRE(std::is_default_constructible_v<std::remove_cvref_t<decltype(ne(val<1>))>>);
}

TEST_CASE("Captured is NOT default constructible when not empty")
{
	STATIC_REQUIRE_FALSE(std::is_default_constructible_v<std::remove_cvref_t<decltype(lt(1))>>);
	STATIC_REQUIRE_FALSE(std::is_default_constructible_v<std::remove_cvref_t<decltype(le(1))>>);
	STATIC_REQUIRE_FALSE(std::is_default_constructible_v<std::remove_cvref_t<decltype(gt(1))>>);
	STATIC_REQUIRE_FALSE(std::is_default_constructible_v<std::remove_cvref_t<decltype(ge(1))>>);
	STATIC_REQUIRE_FALSE(std::is_default_constructible_v<std::remove_cvref_t<decltype(eq(1))>>);
	STATIC_REQUIRE_FALSE(std::is_default_constructible_v<std::remove_cvref_t<decltype(ne(1))>>);
}

TEST_CASE("Captured is NOT default constructible when not default constructible even if empty")
{
	struct empty_t {
		constexpr empty_t(int) { }
	};
	STATIC_REQUIRE_FALSE(
		std::is_default_constructible_v<std::remove_cvref_t<decltype(lt(empty_t(1)))>>);
	STATIC_REQUIRE_FALSE(
		std::is_default_constructible_v<std::remove_cvref_t<decltype(le(empty_t(1)))>>);
	STATIC_REQUIRE_FALSE(
		std::is_default_constructible_v<std::remove_cvref_t<decltype(gt(empty_t(1)))>>);
	STATIC_REQUIRE_FALSE(
		std::is_default_constructible_v<std::remove_cvref_t<decltype(ge(empty_t(1)))>>);
	STATIC_REQUIRE_FALSE(
		std::is_default_constructible_v<std::remove_cvref_t<decltype(eq(empty_t(1)))>>);
	STATIC_REQUIRE_FALSE(
		std::is_default_constructible_v<std::remove_cvref_t<decltype(ne(empty_t(1)))>>);
}
