#pragma once

#include <type_traits>

namespace sudoku {
	namespace detail {
		template <typename T>
		struct type_switch_eval {
			template <typename, typename P>
			[[nodiscard]] constexpr type_switch_eval case_(P) const;

			template <typename>
			constexpr T default_() const;
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

		template <typename T>
		constexpr T default_() const;
	};
}
