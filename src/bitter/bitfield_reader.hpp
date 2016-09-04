// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.
#pragma once

#include <endian/big_endian.hpp>
#include <endian/little_endian.hpp>

#include <cstdint>
#include <vector>
#include <cassert>
#include <typeinfo>

namespace bitter
{
template<typename DataType, uint32_t... Groups>
class bitfield_reader
{
public:
    bitfield_reader(DataType value):
        m_data(value),
        m_data_ptr((uint8_t*)&m_data),
        m_data_size(sizeof(DataType))
    {

    }

    template<typename Type, uint32_t Group>
    Type read()
    {
        auto current_group_size = group_size<Group>();
        auto current_offset = offset<Group>();
        return read_bits_from_offset<Type>(current_group_size, current_offset);
    }

    uint8_t* data_ptr()
    {
        return m_data_ptr;
    }

    template<typename EndianType>
    void convert_endianness()
    {
        EndianType::template put<DataType>(m_data, m_data_ptr);
    }

private:
    uint32_t total_size_of_groups()
    {
        return total_size_of_groups_<Groups...>();
    }

    template<uint32_t Group, uint32_t InputGroup, uint32_t... InputGroups>
    uint32_t total_size_of_groups_()
    {
        return Group + total_size_of_groups_<InputGroup, InputGroups...>();
    }

    template<uint32_t Group>
    uint32_t total_size_of_groups_()
    {
        return Group;
    }

    template<uint32_t Group>
    uint64_t group_size()
    {
        return group_size_<Group, Groups...>();
    }

    template<uint32_t Group, uint32_t NextGroup, uint32_t... InputGroups>
    uint64_t group_size_()
    {
        if((sizeof...(Groups) - Group) == sizeof...(InputGroups) + 1)
        {
            return NextGroup;
        }
        else
        {
            return group_size_<Group, InputGroups...>();
        }
    }

    template<uint32_t Group>
    uint64_t group_size_()
    {
        return 0;
    }

    template<uint32_t Group>
    uint64_t offset()
    {
        return offset_<Group, Groups...>();
    }

    template<uint32_t Group, uint32_t NextGroup, uint32_t... InputGroups>
    uint64_t offset_()
    {
        if((sizeof...(Groups) - Group) == sizeof...(InputGroups) + 1)
        {
            return 0;
        }
        else
        {
            return NextGroup + offset_<Group, InputGroups...>();
        }
    }

    template<uint32_t Group>
    uint64_t offset_()
    {
        return 0;
    }

    template<typename ReturnType>
    ReturnType read_bits_from_offset(uint64_t bits, uint64_t offset)
    {
        assert(bits <= 64);
        ReturnType result = 0;
        for(uint64_t i = 0; i < bits; ++i)
        {
            ReturnType bit = read_bit_at_offset(offset + i);
            result |=  bit << (bits - i - 1);
        }
        return result;
    }

    uint8_t read_bit_at_offset(uint8_t offset)
    {
        auto byte = offset / 8;
        auto position = 7 - (offset % 8);
        return (m_data_ptr[byte] >> position) & 0x1;
    }

private:
    DataType m_data;
    uint8_t* m_data_ptr;
    uint32_t m_data_size;
};
}
