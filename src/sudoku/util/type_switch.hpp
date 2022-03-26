#pragma once

#include <type_traits>

#include <sudoku/util/util.hpp>

namespace sudoku {
	namespace detail {
		template <typename T>
		struct type_switch_eval {
			template <typename U, typename P>
			[[nodiscard]] constexpr type_switch_eval case_( P) const;

			template <typename U, typename P>
			[[nodiscard]] constexpr type_switch_eval case_(type_t<U>, P) const;

			template <typename>
			constexpr T default_() const;

			template <typename U>
			constexpr T default_(type_t<U>) const;
		};
	}

	// A `switch`-like mechanism for types.
	//
	// Example:
	//
	//     using type = decltype(type_switch<10>()
	//                               .case_<int>(lt(val<5>))
	//                               .case_<long>(gt(val<9>))
	//                               .default_<float>());
	//     static_assert(std::is_same_v<type, long>);
	//
	//     // Alternative interface (may be mixed):
	//     using type = decltype(type_switch<10>()
	//                               .case_(type<int>, lt(val<5>))
	//                               .case_(type<long>, gt(val<9>))
	//                               .default_(type<float>));
	//     static_assert(std::is_same_v<type, long>);
	//
	// The value passed as the template parameter (`10` in the example) is passed to each
	// `.case_()`'s predicate, and the type for the first predicate that passes is used. If none of
	// the predicates pass, then the `.default_<T>()` is used. The `.default_` _must_ be provided;
	// it is NOT optional.
	//
	// `type_switch` may only be used in an unevaluated context; it will fail to compile otherwise.
	//
	// Note that the predicates passed _must_ be constexpr callable and default constructible,
	// meaning that all of the state must be encoded in the type. `lt(5)` will NOT work. In
	// practice, this means that only captureless lambdas or function objects which advertise
	// emptiness and constexpr friendliness in this way may be used.
	//
	// Unlike `switch`, this evaluates cases in order, stopping as soon as one is valid.
	template <auto Val>
	struct type_switch {
		template <typename T, typename P>
		[[nodiscard]] constexpr auto case_(P) const
			-> std::conditional_t<P()(Val), detail::type_switch_eval<T>, type_switch>;

		template <typename T, typename P>
		[[nodiscard]] constexpr auto case_(type_t<T>, P) const
			-> std::conditional_t<P()(Val), detail::type_switch_eval<T>, type_switch>;

		template <typename T>
		constexpr T default_() const;

		template <typename T>
		constexpr T default_(type_t<T>) const;
	};
}
