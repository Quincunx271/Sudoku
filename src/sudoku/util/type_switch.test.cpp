#include <sudoku/util/type_switch.hpp>

#include <sudoku/util/comparisons.hpp>
#include <sudoku/util/util.hpp>
#include <type_traits>
#include <utility>

#include <catch2/catch.hpp>

using sudoku::type_switch;

TEST_CASE("Expected style of use")
{
	using sudoku::lt;
	using sudoku::val;

	STATIC_REQUIRE(std::is_same_v<decltype(type_switch<10>()
											   .case_<int>(lt(val<5>))
											   .case_<long>(lt(val<11>))
											   .default_<double>()),
		long>);
}

TEST_CASE("Yields first true case")
{
	STATIC_REQUIRE(std::is_same_v<decltype(type_switch<10>()
											   .case_<int>([](auto) { return false; })
											   .case_<long>([](auto) { return true; })
											   .case_<double>([](auto) { return true; })
											   .default_<float>()),
		long>);
}

TEST_CASE("Can yield the first case")
{
	STATIC_REQUIRE(std::is_same_v<decltype(type_switch<10>()
											   .case_<int>([](auto) { return true; })
											   .case_<long>([](auto) { return true; })
											   .case_<double>([](auto) { return true; })
											   .default_<float>()),
		int>);
}

TEST_CASE("Yields the default if all other options are false")
{
	STATIC_REQUIRE(std::is_same_v<decltype(type_switch<10>()
											   .case_<int>([](auto) { return false; })
											   .case_<long>([](auto) { return false; })
											   .case_<double>([](auto) { return false; })
											   .default_<float>()),
		float>);
	// If there is even one that is true, we don't get to the default.
	STATIC_REQUIRE_FALSE(std::is_same_v<decltype(type_switch<10>()
													 .case_<int>([](auto) { return false; })
													 .case_<long>([](auto) { return false; })
													 .case_<double>([](auto) { return true; })
													 .default_<float>()),
		float>);
}

TEST_CASE("Threads the captured value through each case")
{
	decltype(type_switch<std::true_type{}>()
				 .case_<int>([]<typename B>(B) {
					 static_assert(std::is_same_v<B, std::true_type>);
					 return true;
				 })
				 .default_<long>()) x;
}
