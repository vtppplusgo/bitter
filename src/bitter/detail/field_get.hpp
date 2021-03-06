// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.
#pragma once

#include "field_mask.hpp"

#include <cstdint>
#include <cassert>

namespace bitter
{
/// @brief function for accessing a field based on
///        the provided index
/// @param value is the data give to the reader at initialization
template
<
    typename DataType,
    typename BitNumbering,
    uint32_t Index,
    uint32_t... Sizes
>
typename DataType::type field_get(typename DataType::type value)
{
    uint32_t offset = BitNumbering::template field_offset<Index, Sizes...>();
    typename DataType::type mask = field_mask<DataType, Index, Sizes...>();

    return (value >> offset) & mask;
}
}
