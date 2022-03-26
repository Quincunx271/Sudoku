#pragma once

#include <concepts>
#include <iosfwd>
#include <optional>
#include <utility>

#include <sudoku/util/assert.hpp>

namespace sudoku {
	template <typename T, auto L, auto H>
	class bounded;

	namespace detail {
		template <typename T, typename Char = char>
		concept ostreamable = requires(std::basic_ostream<Char>& out, const T& t)
		{
			// clang-format off
			{ out << t } -> std::same_as<std::basic_ostream<Char>&>;
			// clang-format on
		};

		template <typename T, typename Char = char>
		concept istreamable = requires(std::basic_istream<Char>& in, T& t)
		{
			// clang-format off
			{ in >> t } -> std::same_as<std::basic_istream<Char>&>;
			// clang-format on
		};

		template <typename T, auto L, auto H, typename Char>
		std::basic_ostream<Char>& write(std::basic_ostream<Char>& out, const bounded<T, L, H>& val);

		template <typename T, auto L, auto H, typename Char>
		std::basic_istream<Char>& read(std::basic_istream<Char>& in, bounded<T, L, H>& val);
	}

	template <typename T, auto L, auto H>
	class bounded {
	public:
		constexpr bounded() = default;
		constexpr bounded(T val)
			: val_(std::move(val))
		{
			SUDOKU_DBG_ASSERT(L <= val_);
			SUDOKU_DBG_ASSERT(val_ <= H);
		}
		template <auto L2, auto H2>
		constexpr bounded(bounded<T, L2, H2> val) requires(L <= L2 && H2 <= H)
			: val_(std::move(val).get())
		{ }

		static constexpr std::optional<bounded> parse(T val)
		{
			if (val < L || val > H) return std::nullopt;
			return val;
		}

		constexpr const T& get() const&
		{
			return val_;
		}
		constexpr T& get() &
		{
			return val_;
		}
		constexpr const T&& get() const&&
		{
			return std::move(val_);
		}
		constexpr T&& get() &&
		{
			return std::move(val_);
		}

		constexpr friend bool operator==(const bounded&, const bounded&) = default;
		constexpr friend bool operator==(const bounded& lhs, const T& rhs)
		{
			return lhs.get() == rhs;
		}

		constexpr friend auto operator<=>(const bounded&, const bounded&) = default;
		constexpr friend auto operator<=>(const bounded& lhs, const T& rhs)
		{
			return lhs.get() <=> rhs;
		}

		template <typename Char>
			requires detail::ostreamable<T, Char>
		friend std::basic_ostream<Char>& operator<<(
			std::basic_ostream<Char>& out, const bounded& val)
		{
			return detail::write(out, val);
		}

		template <typename Char>
			requires(detail::istreamable<T, Char>&& std::constructible_from<T>)
		friend std::basic_istream<Char>& operator>>(std::basic_istream<Char>& in, bounded& val)
		{
			return detail::read(in, val);
		}

	private:
		T val_;
	};
}
