#pragma once

#include <ios>

#include <sudoku/util/bounded.hpp>

namespace sudoku::detail {
	template <typename T, auto L, auto H, typename Char>
	std::basic_ostream<Char>& write(std::basic_ostream<Char>& out, const bounded<T, L, H>& val)
	{
		return out << val.get();
	}

	template <typename T, auto L, auto H, typename Char>
	std::basic_istream<Char>& read(std::basic_istream<Char>& in, bounded<T, L, H>& val)
	{
		T x;
		std::basic_istream<Char>& r = in >> x;
		if (r.fail()) return r;

		auto opt = parse(std::move(x));
		if (!opt) {
			r.setstate(std::ios_base::failbit);
			return r;
		}

		val = *std::move(opt);

		return r;
	}
}
