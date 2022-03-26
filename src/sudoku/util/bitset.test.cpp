#include <sudoku/util/bitset.hpp>

#include <climits>

#include <catch2/catch.hpp>

using sudoku::bitset;

template <typename... Ts>
struct typelist { };

using many_bitset_sizes = typelist<bitset<1>, bitset<2>, bitset<3>, bitset<9>, bitset<10>,
	bitset<30>, bitset<32>, bitset<33>, bitset<63>, bitset<64>, bitset<65>, bitset<100>>;

TEST_CASE("sizeof(bitset<...>)")
{
	SECTION("is as small as possible for small sizes")
	{
		static_assert(CHAR_BIT == 8, "These tests only work if CHAR_BIT is 8");
		STATIC_REQUIRE(sizeof(bitset<1>) == 1);
		STATIC_REQUIRE(sizeof(bitset<8>) == 1);
		STATIC_REQUIRE(sizeof(bitset<9>) == 2);
		STATIC_REQUIRE(sizeof(bitset<16>) == 2);
		STATIC_REQUIRE(sizeof(bitset<17>) == 4);
		STATIC_REQUIRE(sizeof(bitset<32>) == 4);
		STATIC_REQUIRE(sizeof(bitset<33>) == 8);
		STATIC_REQUIRE(sizeof(bitset<64>) == 8);
		// Remaining would be arrays.
	}
	SECTION("is roughly proportional to the number of bits used")
	{
		using namespace Catch::literals;
		static constexpr auto largest_word_to_pass = 1024;
		static constexpr auto large = 1'001'002'003;
		static constexpr double epsilon = double(largest_word_to_pass) / large;
		CAPTURE(epsilon);

		CHECK(double(sizeof(bitset<large>)) / double(sizeof(bitset<large / 2>))
			== 2.0_a /**/.epsilon(epsilon));
		CHECK(double(sizeof(bitset<large>)) / double(sizeof(bitset<large / 3>))
			== 3.0_a /**/.epsilon(epsilon));
		CHECK(double(sizeof(bitset<large>)) / double(sizeof(bitset<large * 2>))
			== 0.5_a /**/.epsilon(epsilon));
	}
}

TEMPLATE_LIST_TEST_CASE(
	"sizeof(bitset<...>) holds at least as many bits as requested", "", many_bitset_sizes)
{
	CAPTURE(sizeof(TestType) * CHAR_BIT);
	CAPTURE(TestType().size());
	STATIC_REQUIRE(sizeof(TestType) * CHAR_BIT >= TestType().size());
}

TEMPLATE_LIST_TEST_CASE("Default constructed state", "", many_bitset_sizes)
{
	TestType bits;
	CAPTURE(bits.size());

	SECTION("has all bits false")
	{
		for (std::size_t index = 0; index < bits.size(); ++index) {
			CAPTURE(index);
			CHECK_FALSE(bits.get(index));
		}
	}
}

TEMPLATE_LIST_TEST_CASE("set()", "", many_bitset_sizes)
{
	TestType bits;
	CAPTURE(bits.size());

	SECTION("sets to specified values")
	{
		std::size_t index = bits.size() > 3 ? 3 : 0;
		bits.set(index, true);
		CHECK(bits.get(index));
		bits.set(index, false);
		CHECK_FALSE(bits.get(index));
	}
	SECTION("roundtrips with get()")
	{
		const bool set_to = GENERATE(values({1, 0}));

		for (std::size_t i = 0; i < bits.size(); ++i) {
			bits.set(i, !set_to);
		}

		DYNAMIC_SECTION("set(..., " << std::boolalpha << set_to << ")")
		{
			for (std::size_t i = 0; i < bits.size(); ++i) {
				DYNAMIC_SECTION("Index: " << i)
				{
					bits.set(i, set_to);
					CHECK(bits.get(i) == set_to);
				}
			}
		}
	}
}
