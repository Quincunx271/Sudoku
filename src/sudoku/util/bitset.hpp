#pragma once

#include <array>
#include <climits>
#include <cstddef>
#include <cstdint>

#include <sudoku/util/assert.hpp>
#include <sudoku/util/comparisons.hpp>
#include <sudoku/util/type_switch.hpp>
#include <sudoku/util/util.hpp>

namespace sudoku {
	namespace detail {
		template <std::size_t N>
		using bitset_data_t
			= decltype(type_switch<N>()
						   .case_(type<std::uint8_t>, le(val<CHAR_BIT * sizeof(std::uint8_t)>))
						   .case_(type<std::uint16_t>, le(val<CHAR_BIT * sizeof(std::uint16_t)>))
						   .case_(type<std::uint32_t>, le(val<CHAR_BIT * sizeof(std::uint32_t)>))
						   .default_(type<std::uint64_t>));
	}

	// A custom version of std::bitset which is constexpr friendly.
	template <std::size_t N>
	class bitset {
		// TODO: if intensive work need to be done on this, consider looking at
		// ThePhantomDerpstorm's bit-vector work (itsy bitsy).

		using word_t = detail::bitset_data_t<N>;
		static constexpr std::size_t bits_per_word = CHAR_BIT * sizeof(word_t);
		static constexpr std::size_t num_whole_words = N / bits_per_word;
		static constexpr std::size_t remaining_bits = N % bits_per_word;

		static constexpr std::size_t num_words = num_whole_words + (remaining_bits != 0);

	public:
		constexpr bitset() = default;

		constexpr void set(std::size_t index, bool value)
		{
			const auto [word_idx, bit_idx] = split_index(index);

			const word_t word = words[word_idx];
			const word_t set_mask = word_t(value) << bit_idx;
			const word_t clear_mask = ~word_t(word_t(1) << bit_idx);
			words[word_idx] = word_t(word & clear_mask) | set_mask;
		}

		constexpr bool get(std::size_t index) const
		{
			const auto [word_idx, bit_idx] = split_index(index);

			const word_t mask = word_t(1) << bit_idx;
			return words[word_idx] & mask;
		}

		static constexpr std::size_t size()
		{
			return N;
		}

	private:
		struct bit_addr_t {
			std::size_t word;
			std::size_t bit;
		};

		constexpr bit_addr_t split_index(std::size_t index) const
		{
			SUDOKU_DBG_ASSERT(index < size());

			const bit_addr_t addr{
				.word = index / bits_per_word,
				.bit = index % bits_per_word,
			};

			SUDOKU_DBG_ASSERT(addr.word < words.size());
			SUDOKU_DBG_ASSERT(
				// In the case where we are addressing the last word...
				addr.word < words.size() - 1
				// but only if this last word is partial...
				|| remaining_bits == 0
				// the bit must be one of the valid bits in that last word.
				|| addr.bit < remaining_bits);

			return addr;
		}

		std::array<word_t, num_words> words{};
	};
}
