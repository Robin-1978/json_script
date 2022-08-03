
#include "vm.h"
#include <fmt/color.h>

int main(int argc, char *argv[])
{
    fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold, "Hello, {}!\n", "world");
    vm::Vm vm;
}