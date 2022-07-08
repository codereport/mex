
#pragma once

#include <string>
#include <vector>

#include <functions_unary.hpp>
#include <token.hpp>

auto eval_unary_function(token function, tensor<int> array) -> tensor<int> {
    if (function.u16string_value() == utf8::utf8to16("⍳")) return unary_iota(array);
    if (function.u16string_value() == utf8::utf8to16("⌽")) return unary_reverse(array);
    // add tl::expected for "not supported error"
    return tensor<int>{};
}

auto eval_single_step(std::vector<token> tokens) {
    if (tokens.size() <= 1) return tokens;
    if (tokens.back().type() != token_t::ARRAY) return tokens;
    auto const arr = tokens.back().tensor_value();
    tokens.pop_back();
    // currently, assume only unary functions and arrays
    auto const new_arr = eval_unary_function(tokens.back(), arr);
    tokens.pop_back();
    tokens.push_back(token{new_arr});
    return tokens;
}