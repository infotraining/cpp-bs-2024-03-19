#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <span>

using namespace std::literals;

auto to_underlying(auto m) // generic function
{
    using EnumType = decltype(m);

    std::underlying_type_t<EnumType> index = static_cast<std::underlying_type_t<EnumType>>(m);
    return index;
}

enum DayOfWeek : uint8_t {
    mon = 1,
    tue,
    wed,
    thd,
    fri,
    sat,
    sun
};

TEST_CASE("enums")
{
    DayOfWeek day = tue;

    // std::underlying_type_t<DayOfWeek> index = day; // implicit conversion is allowed
    auto index = to_underlying(day);
    static_assert(std::is_same_v<decltype(index), uint8_t>);
    CHECK(index == 2);

    DayOfWeek another_day = static_cast<DayOfWeek>(7);
    CHECK(another_day == sun);
}

enum class Month {
    jan = 1,
    feb,
    mar,
    apr,
    may,
    jun,
    jul,
    aug,
    sep,
    oct,
    nov,
    dec
};

TEST_CASE("scoped enumerations")
{
    Month m = Month::jan;

    // std::underlying_type_t<Month> index = static_cast<std::underlying_type_t<Month>>(m);
    auto index = to_underlying(m);

    Month another_m = static_cast<Month>(6);
    CHECK(another_m == Month::jun);
}

namespace Explain
{
    enum class byte : uint8_t {};

    constexpr byte operator|(const byte _Left, const byte _Right) noexcept
    {
        // every static_cast is intentional
        return static_cast<byte>(
            static_cast<uint8_t>(static_cast<uint8_t>(_Left) | static_cast<uint8_t>(_Right)));
    }
} // namespace Explain

TEST_CASE("std::byte")
{
    std::byte b1{42};
    std::byte b2{66};

    std::byte b_result = (b1 | ~b2);

    // char result = b1 + b2;

    std::cout << "b_result: " << std::to_integer<int>(b_result) << "\n";

	std::array<std::byte, 8> buffer = {std::byte{0x1}, std::byte{0x2}};
}


void print(float const x, std::span<const std::byte> const bytes)
{
    std::cout << std::setprecision(6) << std::setw(8) << x << " = { "
              << std::hex << std::uppercase << std::setfill('0');
    for (auto const b : bytes)
        std::cout << std::setw(2) << std::to_integer<int>(b) << ' ';
    std::cout << std::dec << "}\n";
}

TEST_CASE("std::byte + std::span")
{
	float data[1]{3.141592f};

    const std::span<const std::byte, 4> const_bytes = std::as_bytes(std::span{data}); 
	print(data[0], const_bytes);

	const std::span<std::byte, 4> writable_bytes = std::as_writable_bytes(std::span{data});
 
    // Change the sign bit that is the MSB (IEEE 754 Floating-Point Standard).
    writable_bytes[3] |= std::byte{0B1000'0000};
	print(data[0], const_bytes);
}