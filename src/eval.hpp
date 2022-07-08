
#pragma once

#include <string>
#include <vector>

#include <error.hpp>
#include <functions_binary.hpp>
#include <functions_unary.hpp>
#include <token.hpp>

auto eval_unary_function(token function, tensor<int> t) -> tensor<int> {
    if (function.u16string_value() == utf8::utf8to16("⍳")) return unary_iota(t);
    if (function.u16string_value() == utf8::utf8to16("⌽")) return unary_reverse(t);
    // add tl::expected for "not supported error"
    return tensor<int>{};
}

auto eval_binary_function(token function, tensor<int> l, tensor<int> r) -> tensor<int> {
    if (function.u16string_value() == utf8::utf8to16("+")) return binary_plus(l, r);
    // add tl::expected for "not supported error"
    return tensor<int>{};
}

auto eval_single_step(std::vector<token> tokens) -> expected_tokens {
    if (tokens.size() <= 1) return tokens;
    if (tokens.back().type() != token_t::ARRAY) return tokens;
    auto const arr = tokens.back().tensor_value();
    tokens.pop_back();
    // currently, assume only functions and arrays (not operators)
    if (is_unary_function(tokens.back())) {
        auto const new_arr = eval_unary_function(tokens.back(), arr);
        tokens.pop_back();
        tokens.push_back(token{new_arr});
    } else if (is_binary_function(tokens.back())) {
        auto const fun = tokens.back();
        tokens.pop_back();
        if (tokens.back().type() != token_t::ARRAY) {
            return make_tokens_error(error_type::DOMAIN, "binary function left argument should be tensor");
        }
        auto const new_arr = eval_binary_function(fun, tokens.back().tensor_value(), arr);
        tokens.pop_back();
        tokens.push_back(token{new_arr});
    }
    return tokens;
}
