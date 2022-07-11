
#pragma once

#include <vector>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/chunk_by.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/transform.hpp>

#include <utf8.h>

#include <combinators.hpp>
#include <token.hpp>

using namespace combinators;

namespace rv = ranges::views;

auto tokenize(std::string expr) {
    auto padded_expr       = utf8::utf8to16(" " + expr + " ");
    auto necessary_chars   = [](auto r) { return r[1] != ' ' or _psi(_and_, ::isdigit)(r[0], r[2]); };
    auto is_digit_or_space = _phi(::isdigit, _or_, _eq(' '));
    auto range_to_token    = [](auto r) { return token{std::u16string(r.begin(), r.end())}; };

    return padded_expr                                     //
           | rv::sliding(3)                                //
           | rv::filter(necessary_chars)                   //
           | rv::transform([](auto r) { return r[1]; })    //
           | rv::chunk_by(_psi(_and_, is_digit_or_space))  //
           | rv::transform(range_to_token)                 //
           | ranges::to<std::vector<token>>;
}
