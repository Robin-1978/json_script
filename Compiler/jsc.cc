#include "parser.h"

#include <lexy/action/parse.hpp> // lexy::parse
#include <lexy/input/file.hpp>   // lexy::read_file
#include <lexy_ext/report_error.hpp> // lexy_ext::report_error

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::fprintf(stderr, "usage: %s <filename>", argv[0]);
        return 1;
    }

    // We're requiring UTF-8 input.
    auto file = lexy::read_file<lexy::utf8_encoding>(argv[1]);
    if (!file)
    {
        std::fprintf(stderr, "file '%s' not found", argv[1]);
        return 1;
    }

    auto json = lexy::parse<grammar::json_root>(file.buffer(), lexy_ext::report_error);
    if (json.has_value())
    {
        
    }

    if (!json)
        return 2;
}