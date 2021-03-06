// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file

#include <bitter/lsb0_reader.hpp>
#include <bitter/msb0_reader.hpp>

#include <cstdint>
#include <typeinfo>

#include <gtest/gtest.h>

TEST(test_bit_reader, read_integer)
{
    {
        uint8_t input = 0x0FU;
        auto reader = bitter::lsb0_reader<uint8_t, 4, 4>(input);

        auto value = reader.field<0>().as<uint8_t>();
        EXPECT_EQ(0xFU, value);
        value = reader.field<1>().as<uint8_t>();
        EXPECT_EQ(0x0U, value);
    }
    {
        uint16_t input = 0x0FF0U;
        auto reader = bitter::lsb0_reader<uint16_t, 8, 8>(input);

        auto value = reader.field<0>().as<uint8_t>();
        EXPECT_EQ(0xF0U, value);
        value = reader.field<1>().as<uint8_t>();
        EXPECT_EQ(0x0FU, value);
    }
    {
        uint32_t input = 0x0FF0FF00U;
        auto reader = bitter::lsb0_reader<uint32_t, 16, 16>(input);

        auto value = reader.field<0>().as<uint16_t>();
        EXPECT_EQ(0xFF00U, value);
        value = reader.field<1>().as<uint16_t>();
        EXPECT_EQ(0x0FF0U, value);
    }
    {
        uint64_t input = 0xFFF0FF000FF0FF00U;
        auto reader = bitter::lsb0_reader<uint64_t, 32, 32>(input);

        auto value = reader.field<0>().as<uint32_t>();
        EXPECT_EQ(0x0FF0FF00U, value);
        value = reader.field<1>().as<uint32_t>();
        EXPECT_EQ(0xFFF0FF00U, value);
    }
}

TEST(test_bit_reader, read_bit)
{
    uint32_t input = 0x0FF0FF00U;

    {
        auto reader = bitter::lsb0_reader<bitter::u32, 8, 8, 8, 8>(input);

        auto value = reader.field<0>().as<uint8_t>();
        EXPECT_EQ(0x00U, value);
        value = reader.field<1>().as<uint8_t>();
        EXPECT_EQ(0xFFU, value);
        value = reader.field<2>().as<uint8_t>();
        EXPECT_EQ(0xF0U, value);
        value = reader.field<3>().as<uint8_t>();
        EXPECT_EQ(0x0FU, value);
    }

    {
        auto reader = bitter::msb0_reader<bitter::u32, 8, 8, 8, 8>(input);

        auto value = reader.field<0>().as<uint8_t>();
        EXPECT_EQ(0x0FU, value);
        value = reader.field<1>().as<uint8_t>();
        EXPECT_EQ(0xF0U, value);
        value = reader.field<2>().as<uint8_t>();
        EXPECT_EQ(0xFFU, value);
        value = reader.field<3>().as<uint8_t>();
        EXPECT_EQ(0x00U, value);
    }

}

TEST(test_bit_reader, read_bit1)
{

    uint32_t input = 0x0FF0FF00U;

    {
        auto reader = bitter::lsb0_reader<bitter::u32, 16, 16>(input);

        auto value = reader.field<0>().as<uint16_t>();
        EXPECT_EQ(0xFF00U, value);
        value = reader.field<1>().as<uint16_t>();
        EXPECT_EQ(0x0FF0U, value);
    }

    {
        auto reader = bitter::msb0_reader<bitter::u32, 16, 16>(input);

        auto value = reader.field<0>().as<uint16_t>();
        EXPECT_EQ(0x0FF0U, value);
        value = reader.field<1>().as<uint16_t>();
        EXPECT_EQ(0xFF00U, value);
    }
}

TEST(test_bit_reader, read_bit_u24)
{

    uint32_t input = 0xF0FF00U;

    {
        auto reader = bitter::lsb0_reader<bitter::u24, 16, 8>(input);

        auto value = reader.field<0>().as<uint16_t>();
        EXPECT_EQ(0xFF00U, value);
        value = reader.field<1>().as<uint8_t>();
        EXPECT_EQ(0xF0U, value);
    }

    {
        auto reader = bitter::msb0_reader<bitter::u24, 16, 8>(input);

        auto value = reader.field<0>().as<uint16_t>();
        EXPECT_EQ(0xF0FFU, value);
        value = reader.field<1>().as<uint8_t>();
        EXPECT_EQ(0x00, value);
    }
}


TEST(test_bit_reader, read_bit3)
{
    uint64_t input = 0xA50FF0A50FF0FF00U;

    {
        auto reader = bitter::lsb0_reader<bitter::u64, 32, 32>(input);

        auto value = reader.field<0>().as<uint32_t>();
        EXPECT_EQ(0x0FF0FF00U, value);
        value = reader.field<1>().as<uint32_t>();
        EXPECT_EQ(0xA50FF0A5U, value);
    }

    {
        auto reader = bitter::msb0_reader<bitter::u64, 32, 32>(input);

        auto value = reader.field<0>().as<uint32_t>();
        EXPECT_EQ(0xA50FF0A5U, value);
        value = reader.field<1>().as<uint32_t>();
        EXPECT_EQ(0x0FF0FF00U, value);
    }
}

TEST(test_bit_reader, read_bit4)
{
    uint64_t input = 0xA50FF0A50FF0FF00U;
    {
        auto reader = bitter::lsb0_reader<bitter::u64, 64>(input);

        auto value = reader.field<0>().as<uint64_t>();
        EXPECT_EQ(0xA50FF0A50FF0FF00U, value);
    }
    {
        auto reader = bitter::msb0_reader<bitter::u64, 64>(input);

        auto value = reader.field<0>().as<uint64_t>();
        EXPECT_EQ(0xA50FF0A50FF0FF00U, value);
    }
}

TEST(test_bit_reader, read_bit5)
{
    uint8_t input = 0xF1;
    {
        auto reader = bitter::lsb0_reader<bitter::u8, 1, 7>(input);

        auto value = reader.field<0>().as<bool>();
        EXPECT_TRUE(value);

        auto second_value = reader.field<1>().as<uint8_t>();
        EXPECT_EQ(0b1111000, second_value);
    }
    {
        auto reader = bitter::msb0_reader<bitter::u8, 1, 7>(input);

        auto value = reader.field<0>().as<bool>();
        EXPECT_TRUE(value);

        auto second_value = reader.field<1>().as<uint8_t>();
        EXPECT_EQ(0b1110001, second_value);
    }
}

TEST(test_bit_reader, read_bit6)
{
    {
        uint8_t input = 0b10000001;
        auto reader = bitter::lsb0_reader<bitter::u8, 1, 7>(input);

        auto value = reader.field<0>().as<bool>();
        EXPECT_TRUE(value);

        auto second_value = reader.field<1>().as<uint8_t>();
        EXPECT_EQ(0b1000000, second_value);
    }
    {
        uint8_t input = 0b10000001;
        auto reader = bitter::msb0_reader<bitter::u8, 1, 7>(input);

        auto value = reader.field<0>().as<bool>();
        EXPECT_TRUE(value);

        auto second_value = reader.field<1>().as<uint8_t>();
        EXPECT_EQ(0b0000001, second_value);
    }
}

TEST(test_bit_reader, read_bit9)
{
    {
        uint8_t input = 0b10110101;
        auto reader = bitter::lsb0_reader<
                      bitter::u8, 1, 1, 1, 1, 1, 1, 1, 1>(input);

        auto value = reader.field<0>().as<bool>();
        EXPECT_TRUE(value);

        value = reader.field<1>().as<bool>();
        EXPECT_FALSE(value);

        value = reader.field<2>().as<bool>();
        EXPECT_TRUE(value);

        value = reader.field<3>().as<bool>();
        EXPECT_FALSE(value);

        value = reader.field<4>().as<bool>();
        EXPECT_TRUE(value);

        value = reader.field<5>().as<bool>();
        EXPECT_TRUE(value);

        value = reader.field<6>().as<bool>();
        EXPECT_FALSE(value);

        value = reader.field<7>().as<bool>();
        EXPECT_TRUE(value);
    }
    {
        uint8_t input = 0b10110101;
        auto reader = bitter::msb0_reader<
                      bitter::u8, 1, 1, 1, 1, 1, 1, 1, 1>(input);

        auto value = reader.field<0>().as<bool>();
        EXPECT_TRUE(value);

        value = reader.field<1>().as<bool>();
        EXPECT_FALSE(value);

        value = reader.field<2>().as<bool>();
        EXPECT_TRUE(value);

        value = reader.field<3>().as<bool>();
        EXPECT_TRUE(value);

        value = reader.field<4>().as<bool>();
        EXPECT_FALSE(value);

        value = reader.field<5>().as<bool>();
        EXPECT_TRUE(value);

        value = reader.field<6>().as<bool>();
        EXPECT_FALSE(value);

        value = reader.field<7>().as<bool>();
        EXPECT_TRUE(value);
    }
}

TEST(test_bit_reader, test_const)
{
    uint8_t input = 0x01;
    {
        const auto reader = bitter::lsb0_reader<
                            bitter::u8, 1, 1, 1, 1, 1, 1, 1, 1>(input);

        const auto field1 = reader.field<0>();
        auto value1 = field1.as<bool>();
        EXPECT_TRUE(value1);

        const auto field2 = reader.field<7>();
        auto value2 = field2.as<bool>();
        EXPECT_FALSE(value2);
    }
    {
        const auto reader = bitter::msb0_reader<
                            bitter::u8, 1, 1, 1, 1, 1, 1, 1, 1>(input);

        const auto field1 = reader.field<0>();
        auto value1 = field1.as<bool>();
        EXPECT_FALSE(value1);

        const auto field2 = reader.field<7>();
        auto value2 = field2.as<bool>();
        EXPECT_TRUE(value2);
    }
}
