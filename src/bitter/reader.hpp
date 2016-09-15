// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.
#pragma once

#include "helper_functions.hpp"

#include <cstdint>
#include <vector>
#include <cassert>
#include <typeinfo>

namespace bitter
{
template<typename DataType, uint32_t... Sizes>
struct reader
{
    reader(DataType value) : m_value(value)
    {
        static_assert(size_in_bits<DataType>() == sum_sizes<Sizes...>(),
                      "stop it..");
    }

    template<uint32_t Index>
    uint32_t read()
    {
        return field_get<DataType, Index, Sizes...>(m_value);
    }

    template<uint32_t Index, class FieldType>
    FieldType read_as()
    {
        // Doble paran needed because assert is a macro on some platforms, and
        // they don't like the commas in the templates
        assert((field_size_in_bits<Index, Sizes...>()) <=
               size_in_bits<FieldType>());
        return (FieldType) field_get<DataType, Index, Sizes...>(m_value);
    }

    DataType m_value;
};
}
