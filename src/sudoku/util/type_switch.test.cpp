#include <sudoku/util/type_switch.hpp>

#include <sudoku/util/comparisons.hpp>
#include <sudoku/util/util.hpp>
#include <type_traits>
#include <utility>

#include <catch2/catch.hpp>

using sudoku::type;
using sudoku::type_switch;

TEST_CASE("Expected style of use")
{
	using sudoku::lt;
	using sudoku::val;

	SECTION("Template args")
	{
		STATIC_REQUIRE(std::is_same_v<decltype(type_switch<10>()
												   .case_<int>(lt(val<5>))
												   .case_<long>(lt(val<11>))
												   .default_<double>()),
			long>);
	}
	SECTION("Deduced")
	{
		STATIC_REQUIRE(std::is_same_v<decltype(type_switch<10>()
												   .case_(type<int>, lt(val<5>))
												   .case_(type<long>, lt(val<11>))
												   .default_(type<double>)),
			long>);
	}
}

TEST_CASE("Yields first true case")
{
	SECTION("Template args")
	{
		STATIC_REQUIRE(std::is_same_v<decltype(type_switch<10>()
												   .case_<int>([](auto) { return false; })
												   .case_<long>([](auto) { return true; })
												   .case_<double>([](auto) { return true; })
												   .default_<float>()),
			long>);
	}
	SECTION("Deduced")
	{
		STATIC_REQUIRE(std::is_same_v<decltype(type_switch<10>()
												   .case_(type<int>, [](auto) { return false; })
												   .case_(type<long>, [](auto) { return true; })
												   .case_(type<double>, [](auto) { return true; })
												   .default_(type<float>)),
			long>);
	}
}

TEST_CASE("Can yield the first case")
{
	SECTION("Template args")
	{
		STATIC_REQUIRE(std::is_same_v<decltype(type_switch<10>()
												   .case_<int>([](auto) { return true; })
												   .case_<long>([](auto) { return true; })
												   .case_<double>([](auto) { return true; })
												   .default_<float>()),
			int>);
	}
	SECTION("Deduced")
	{
		STATIC_REQUIRE(std::is_same_v<decltype(type_switch<10>()
												   .case_(type<int>, [](auto) { return true; })
												   .case_(type<long>, [](auto) { return true; })
												   .case_(type<double>, [](auto) { return true; })
												   .default_(type<float>)),
			int>);
	}
}

TEST_CASE("Yields the default if all other options are false")
{
	SECTION("Template args")
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
	SECTION("Deduced")
	{
		STATIC_REQUIRE(std::is_same_v<decltype(type_switch<10>()
												   .case_(type<int>, [](auto) { return false; })
												   .case_(type<long>, [](auto) { return false; })
												   .case_(type<double>, [](auto) { return false; })
												   .default_(type<float>)),
			float>);
		// If there is even one that is true, we don't get to the default.
		STATIC_REQUIRE_FALSE(
			std::is_same_v<decltype(type_switch<10>()
										.case_(type<int>, [](auto) { return false; })
										.case_(type<long>, [](auto) { return false; })
										.case_(type<double>, [](auto) { return true; })
										.default_(type<float>)),
				float>);
	}
}

TEST_CASE("Threads the captured value through each case")
{
	SECTION("Template args")
	{
		decltype(type_switch<std::true_type{}>()
					 .case_<int>([]<typename B>(B) {
						 static_assert(std::is_same_v<B, std::true_type>);
						 return true;
					 })
					 .default_<long>()) x;
	}
	SECTION("Deduced")
	{
		decltype(type_switch<std::true_type{}>()
					 .case_(type<int>,
						 []<typename B>(B) {
							 static_assert(std::is_same_v<B, std::true_type>);
							 return true;
						 })
					 .default_(type<long>)) x;
	}
}
