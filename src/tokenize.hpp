
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
  auto padded_expr = utf8::utf8to16(" " + expr + " ");

  auto meaningful_space_or_nonspace = [](auto rng) {
    return rng[1] != ' ' or _psi(_and, ::isdigit)(rng[0], rng[2]);
  };

  auto is_digit_or_space = _phi(::isdigit, _or, _eq(' '));

  auto temp = padded_expr                                       //
              | rv::sliding(3)                                  //
              | rv::filter(meaningful_space_or_nonspace)        //
              | rv::transform([](auto rng) { return rng[1]; })  //
              | rv::chunk_by(_psi(_and, is_digit_or_space))     //
              | ranges::to<std::vector<std::u16string>>;

  return temp                                              //
         | rv::transform([](auto s) { return token{s}; })  //
         | ranges::to<std::vector<token>>;
}
