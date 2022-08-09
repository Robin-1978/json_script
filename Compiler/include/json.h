#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <map>

namespace json
{
    struct value
    {
        virtual ~value() = default;
        using ptr = std::unique_ptr<value>;
    };

    template <typename T>
    struct basic_value : value
    {
        using ptr = std::unique_ptr<basic_value>;
        using value_t = T;
        value_t value;
        basic_value() = default;
        basic_value(value_t value) : value(std::move(value)) {}
    };

    using null = basic_value<nullptr_t>;
    using boolean = basic_value<bool>;
    using string = basic_value<std::string>;
    using number = basic_value<std::variant<int64_t, double>>;
    using array = basic_value<std::vector<value::ptr>>;
    using object = basic_value<std::map<std::string, value::ptr>>;
}