// Simple syntax for partially applied comparisons.
//
// When using algorithms or other higher order functions, it is often necessary to provide a
// predicate that has a fixed value for one side of the comparison operators. Haskell has a
// convenient syntax for this:
//
//   filter (>2) list
//
// The function objects provided in this header provide a similar syntax:
//
//   erase_if(vec, gt(2));
//
// This header provides the following function objects:
//  - lt: performs "<"; see std::less.
//  - le: performs "<="; see std::less_equal.
//  - gt: performs ">"; see std::greater.
//  - ge: performs ">="; see std::greater_equal.
//  - eq: performs "=="; see std::equal_to.
//  - ne: performs "!="; see std::not_equal_to.
//
// With any of these function objects as `op`:
//  - `op(a, b) == a <op> b`
//  - `op(x)` is a partially applied function such that `op(x)(y) == y <op> x`. This means that
//    `op(x)` performs the operation it "reads as"; e.g. `lt(2)` is a predicate testing "is less
//    than 2".
//
// These function objects have the following properties:
//  - is_transparent: they are transparent comparators like std::less<> is.
//  - Totally ordered for pointers. It is defined behavior to compare pointers from different arrays
//    using these functions; they will use the total ordering provided by std::less and friends.

#pragma once

#include <functional>
#include <type_traits>
#include <utility>

namespace sudoku {
	namespace detail {
		// Evaluates `Fn`---a comparison object template---to be pointer-safe.
		// E.g.:
		// - ptr_safe_cmp_t<std::less, int*, int*> -> std::less<int*>
		// - ptr_safe_cmp_t<std::less, int, int> -> std::less<>
		//
		// This allows us to always have a total ordering.
		template <template <typename...> class Fn, typename Lhs, typename Rhs>
		using ptr_safe_cmp_t = std::conditional_t<
			std::is_pointer_v<std::remove_cvref_t<
				Lhs>> && std::is_same_v<std::remove_cvref_t<Lhs>, std::remove_cvref_t<Rhs>>,
			Fn<Lhs>, Fn<>>;

		// Provides a nice "currying" syntax for the comparators.
		// This flips the order, so that it reads cleanly: remove_if(..., lt(1)) removes everything
		// less than 1.
		template <template <typename...> class Fn>
		struct curried_cmp_t {
			using is_transparent = void;

			[[nodiscard]] constexpr auto operator()(auto rhs) const
			{
				return
					[rhs = std::move(rhs)](const auto& lhs) { return curried_cmp_t()(lhs, rhs); };
			}

			template <typename Lhs, typename Rhs>
			[[nodiscard]] constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const
			{
				return detail::ptr_safe_cmp_t<Fn, Lhs, Rhs>()(lhs, rhs);
			}
		};
	}

	struct lt_t : detail::curried_cmp_t<std::less> {
	} constexpr lt;
	struct le_t : detail::curried_cmp_t<std::less_equal> {
	} constexpr le;
	struct gt_t : detail::curried_cmp_t<std::greater> {
	} constexpr gt;
	struct ge_t : detail::curried_cmp_t<std::greater_equal> {
	} constexpr ge;
	struct eq_t : detail::curried_cmp_t<std::equal_to> {
	} constexpr eq;
	struct ne_t : detail::curried_cmp_t<std::not_equal_to> {
	} constexpr ne;
}
