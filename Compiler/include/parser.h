#pragma once

#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>

#include "json.h"

namespace grammar
{
    namespace dsl = lexy::dsl;
    struct json_value;
    struct null : lexy::token_production
    {
        static constexpr auto rule = LEXY_LIT("null");
        static constexpr auto value = lexy::new_<json::null, json::value::ptr>;
    };

    struct boolean : lexy::token_production
    {
        struct true_ : lexy::transparent_production
        {
            static constexpr auto rule = LEXY_LIT("true");
            static constexpr auto value = lexy::constant(true);
        };
        struct false_ : lexy::transparent_production
        {
            static constexpr auto rule = LEXY_LIT("false");
            static constexpr auto value = lexy::constant(false);
        };

        static constexpr auto rule = dsl::p<true_> | dsl::p<false_>;
        static constexpr auto value = lexy::new_<json::boolean, json::value::ptr>;
    };
    
    /*
    struct number : lexy::token_production
    {
        struct number_integer : lexy::transparent_production
        {
            static constexpr auto rule = LEXY_LIT("-?[0-9]+");
            static constexpr auto value = lexy::as_integer<std::int64_t>;
        };

        struct number_float
        {
            static constexpr auto rule = LEXY_LIT("-?[0-9]*\\.[0-9]+");
            //static constexpr auto value =  lexy::new_<json::number, json::value::ptr>;
        };

        //static constexpr auto rule = dsl::peek(dsl::lit_c<'-'> / dsl::digit<>) >> dsl::p<number_integer> + dsl::opt(dsl::p<number_float>);
        static constexpr auto rule = dsl::p<number_integer>;
        static constexpr auto value = lexy::as_integer<int64_t> >> lexy::new_<json::number, json::value::ptr>;
    };
    */

   
    struct string : lexy::token_production
    {
        struct invalid_char
        {
            static LEXY_CONSTEVAL auto name()
            {
                return "invalid character in string literal";
            }
        };

        // A mapping of the simple escape sequences to their replacement values.
        static constexpr auto escaped_symbols = lexy::symbol_table<char> //
                                                    .map<'"'>('"')
                                                    .map<'\\'>('\\')
                                                    .map<'/'>('/')
                                                    .map<'b'>('\b')
                                                    .map<'f'>('\f')
                                                    .map<'n'>('\n')
                                                    .map<'r'>('\r')
                                                    .map<'t'>('\t');

        // In JSON, a Unicode code point can be specified by its encoding in UTF-16:
        // * code points <= 0xFFFF are specified using `\uNNNN`.
        // * other code points are specified by two surrogate UTF-16 sequences.
        // However, we don't combine the two surrogates into the final code point,
        // instead keep them separate and require a later pass that merges them if necessary.
        // (This behavior is allowed by the standard).
        struct code_point_id
        {
            // We parse the integer value of a UTF-16 code unit.
            static constexpr auto rule = LEXY_LIT("u") >> dsl::code_unit_id<lexy::utf16_encoding, 4>;
            // And convert it into a code point, which might be a surrogate.
            static constexpr auto value = lexy::construct<lexy::code_point>;
        };

        static constexpr auto rule = []
        {
            // Everything is allowed inside a string except for control characters.
            auto code_point = (-dsl::unicode::control).error<invalid_char>;

            // Escape sequences start with a backlash and either map one of the symbols,
            // or a Unicode code point.
            auto escape = dsl::backslash_escape.symbol<escaped_symbols>().rule(dsl::p<code_point_id>);

            // String of code_point with specified escape sequences, surrounded by ".
            // We abort string parsing if we see a newline to handle missing closing ".
            return dsl::quoted.limit(dsl::ascii::newline)(code_point, escape);
        }();

        static constexpr auto value = lexy::as_string<std::string, lexy::utf8_encoding> >> lexy::new_<json::string, json::value::ptr>;
    };

   struct key_name : lexy::token_production
    {
        struct invalid_char
        {
            static LEXY_CONSTEVAL auto name()
            {
                return "invalid character in string literal";
            }
        };

        // A mapping of the simple escape sequences to their replacement values.
        static constexpr auto escaped_symbols = lexy::symbol_table<char> //
                                                    .map<'"'>('"')
                                                    .map<'\\'>('\\')
                                                    .map<'/'>('/')
                                                    .map<'b'>('\b')
                                                    .map<'f'>('\f')
                                                    .map<'n'>('\n')
                                                    .map<'r'>('\r')
                                                    .map<'t'>('\t');

        // In JSON, a Unicode code point can be specified by its encoding in UTF-16:
        // * code points <= 0xFFFF are specified using `\uNNNN`.
        // * other code points are specified by two surrogate UTF-16 sequences.
        // However, we don't combine the two surrogates into the final code point,
        // instead keep them separate and require a later pass that merges them if necessary.
        // (This behavior is allowed by the standard).
        struct code_point_id
        {
            // We parse the integer value of a UTF-16 code unit.
            static constexpr auto rule = LEXY_LIT("u") >> dsl::code_unit_id<lexy::utf16_encoding, 4>;
            // And convert it into a code point, which might be a surrogate.
            static constexpr auto value = lexy::construct<lexy::code_point>;
        };

        static constexpr auto rule = []
        {
            // Everything is allowed inside a string except for control characters.
            auto code_point = (-dsl::unicode::control).error<invalid_char>;

            // Escape sequences start with a backlash and either map one of the symbols,
            // or a Unicode code point.
            auto escape = dsl::backslash_escape.symbol<escaped_symbols>().rule(dsl::p<code_point_id>);

            // String of code_point with specified escape sequences, surrounded by ".
            // We abort string parsing if we see a newline to handle missing closing ".
            return dsl::quoted.limit(dsl::ascii::newline)(code_point, escape);
        }();

        static constexpr auto value = lexy::as_string<std::string, lexy::utf8_encoding>;
    };

    struct unexpected_trailing_comma
    {
        static constexpr auto name = "unexpected trailing comma";
    };

    // A json value that is an array.
    struct array
    {
        // A (potentially empty) list of json values, seperated by comma and surrounded by square
        // brackets.
        static constexpr auto rule = dsl::square_bracketed
                                         .opt_list(dsl::recurse<json_value>,
                                                   // Trailing seperators are not allowed.
                                                   // Use `dsl::trailing_sep()` if you want to allow it.
                                                   dsl::sep(dsl::comma).trailing_error<unexpected_trailing_comma>);

        static constexpr auto value
        = lexy::as_list<std::vector<json::value::ptr>> >> lexy::callback<json::value::ptr>(
              [](lexy::nullopt = {}) {
                  return std::make_unique<json::array>();
              },
              [](auto&& children) {
                  return std::make_unique<json::array>(LEXY_MOV(children));
              });
    };

    
    // A json value that is an object.
    
    struct object
    {
        static constexpr auto rule = []
        {
            // We try parsing the colon. This means that a missing colon raises an error, which is then
            // caught and parsing continues as if nothing happens. Without the try, parsing the current
            // item would be canceled immediately.
            auto item = dsl::p<key_name> + dsl::try_(dsl::colon) + dsl::recurse<json_value>;

            // Trailing seperators are not allowed.
            // Use `dsl::trailing_sep()` if you want to allow it.
            auto sep = dsl::sep(dsl::comma).trailing_error<unexpected_trailing_comma>;

            // A (potentially empty) list of items, seperated by comma and surrounded by curly brackets.
            return dsl::curly_bracketed.opt_list(item, sep);
        }();



        static constexpr auto value
        = lexy::as_collection<std::map<std::string, json::value::ptr>> >> lexy::callback<json::value::ptr>(
              [](lexy::nullopt = {}) {
                  return std::make_unique<json::object>();
              },
              [](auto&& children) {
                  return std::make_unique<json::object>(LEXY_MOV(children));
              });
    };
    
    // A json value.
    struct json_value : lexy::transparent_production
    {
        static constexpr auto name = "json value";

        struct expected_json_value
        {
            static LEXY_CONSTEVAL auto name()
            {
                return "expected json value";
            }
        };

        static constexpr auto rule = []
        {
            auto primitive = dsl::p<null> | dsl::p<boolean> /*| dsl::p<number> */| dsl::p<string>;
            auto complex = dsl::p<object> | dsl::p<array>;

            return primitive | complex | dsl::error<expected_json_value>;
        }();
        static constexpr auto value = lexy::forward<json::value::ptr>;

    };

    // Entry point of the production.
    struct json_root
    {
        // The maximum level of nesting of JSON data structures,
        // i.e. how many `dsl::recurse` calls are allowed.
        // json.org comes with a test suite that checks for this nesting level.
        static constexpr auto max_recursion_depth = 19;

        // Whitespace is a sequence of space, tab, carriage return, or newline.
        // Add your comment syntax here.
        static constexpr auto whitespace = dsl::ascii::space / dsl::ascii::newline;

        static constexpr auto rule = (dsl::p<array> | dsl::p<object>) + dsl::eof;
        static constexpr auto value = lexy::forward<json::value::ptr>;
    };

}