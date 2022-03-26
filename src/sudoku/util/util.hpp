#pragma once

#include <utility>

namespace sudoku {
	template <auto V>
	using val_t = std::integral_constant<decltype(V), V>;

	template <auto V>
	constexpr val_t<V> val;
}
