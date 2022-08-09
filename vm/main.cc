
#include "vm.h"

#include <iostream>

#include <fmt/color.h>
#include <magic_enum.hpp>

enum class Color : int
{
    RED = -10,
    BLUE = 0,
    GREEN = 10
};
template <typename E>
auto to_integer(magic_enum::Enum<E> value)
{
    // magic_enum::Enum<E> - C++17 Concept for enum type.
    return static_cast<magic_enum::underlying_type_t<E>>(value);
}

int main(int argc, char *argv[])
{
    fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold, "Hello, {}!\n", "world");
    vm::Vm vm;
    Color c1 = Color::RED;
    auto c1_name = magic_enum::enum_name(c1);
    std::cout << c1_name << std::endl; // RED
}