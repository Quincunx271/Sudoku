#pragma once

#include <type_traits>
#include <utility>

namespace sudoku {
	template <auto V>
	using val_t = std::integral_constant<decltype(V), V>;

	template <auto V>
	constexpr val_t<V> val;

	template <typename T>
	using type_t = std::type_identity<T>;

	template <typename T>
	constexpr type_t<T> type;
}
