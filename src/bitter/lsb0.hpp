// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.
#pragma once

#include <cstdint>

namespace bitter
{
/// @brief Calculates the bit shift offset into a value given that bitter
///        is configured in LSB 0 numbering mode (see README.rst for more
///        information).
struct lsb0
{
    /// The basic implementation in LSB 0 mode works according to the
    /// following observation.
    ///
    /// If we want to access a given field at index X then we need to
    /// shift right past the fields at index X-1, ..., 0 etc.
    ///
    /// Example:
    ///
    /// field index: 3     2         1         0
    /// field size:  1     2         3         2
    /// bit index:   7   6   5   4   3   2   1   0
    ///            +---+-------+-----------+-------+
    ///            | 0 | 1   0 | 1   1   1 | 0   0 |
    ///            +---+-------+-----------+-------+
    ///                                          ^
    ///                                          |
    ///               least significant +--------+
    ///               bit
    ///
    /// Say we want to access field index 2, then we need to shift it
    /// right past field 1 and 0. They have size 3+2 = 5 bits.
    ///
    /// We do this by implementing a counter which iterates through the
    /// field sizes accumulating the sizes until we reach the index we are
    /// interested in.
    ///
    template<uint32_t Index, uint32_t... Sizes>
    static uint32_t field_offset()
    {
        return count_to_field_offset<Index, 0, Sizes...>();
    }

private:

    template
    <
        uint32_t Index,
        uint32_t Counter,
        uint32_t Size0
    >
    static uint32_t count_to_field_offset()
    {
        static_assert(Index == Counter, "");
        return 0;
    }

    template
    <
        uint32_t Index,
        uint32_t Counter,
        uint32_t Size0,
        uint32_t Size1,
        uint32_t... Sizes
    >
    static uint32_t count_to_field_offset()
    {
        if (Index == Counter)
        {
            return 0;
        }
        else
        {
            return Size0 + count_to_field_offset<
                   Index,
                   Counter + (Counter < Index ? 1 : 0),
                   Size1,
                   Sizes...>();
        }
    }
};
}
