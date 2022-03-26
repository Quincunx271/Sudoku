#include <sudoku/util/bounded.hpp>
#include <sudoku/util/bounded.io.hpp>

#include <sstream>
#include <string_view>

using sudoku::bounded;

TEMPLATE_TEST_CASE("bounded ios", "", char, wchar_t)
{
	using Char = TestType;

	static constexpr auto to_string
		= [](std::string_view str) { return std::basic_string<Char>(str.begin(), str.end()); };

	SECTION("can be printed")
	{
		std::basic_ostringstream<Char> oss;

		oss << bounded<int, 1, 3>(2);
		CHECK(oss.str() == to_string("2"));
	}
	SECTION("can be parsed")
	{
		std::basic_istringstream<Char> iss;
		bounded<int, 0, 10> bounded;

		SECTION("valid")
		{
			iss.str(to_string("5"));
			bounded = 0;
			iss >> bounded;
			CHECK_FALSE(iss.fail());
			CHECK(iss.eof());
		}
		SECTION("invalid")
		{
			iss.str(to_string("asdf"));
			bounded = 0;
			iss >> bounded;
			CHECK(iss.fail());
			CHECK(bounded == 0); // Shouldn't be changed.
		}
		SECTION("out of bounds")
		{
			iss.str(to_string("100"));
			bounded = 0;
			iss >> bounded;
			CHECK(iss.fail());
			CHECK(bounded == 0); // Shouldn't be changed.
		}
	}
}
