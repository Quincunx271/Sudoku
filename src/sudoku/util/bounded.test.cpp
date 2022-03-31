#include <sudoku/util/bounded.hpp>

#include <concepts>

#include <catch2/catch.hpp>

using sudoku::bounded;

TEST_CASE("bounded's bounds")
{
	SECTION("can be extended")
	{
		STATIC_REQUIRE(std::convertible_to<bounded<int, 1, 2>, bounded<int, 1, 3>>);
		STATIC_REQUIRE(std::convertible_to<bounded<int, 1, 2>, bounded<int, 0, 2>>);
	}
	SECTION("cannot be shrunk")
	{
		STATIC_REQUIRE_FALSE(std::convertible_to<bounded<int, 1, 2>, bounded<int, 1, 1>>);
		STATIC_REQUIRE_FALSE(std::convertible_to<bounded<int, 1, 2>, bounded<int, 2, 2>>);
	}
}

// TODO: add more tests.

// TODO: add test cases for value categories on .get().
