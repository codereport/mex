
#pragma once

#include <string>
#include <variant>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>

#include <utf8.h>

#include <tensor.hpp>

namespace rv = ranges::views;

enum class token_t { ARRAY, FUNCTION, OPERATOR, PAREN };

auto is_operator(std::u16string const& s) -> bool {
  // will be updated as operators are added
  return s == utf8::utf8to16("/") or s == utf8::utf8to16("\\");
}

auto u16_to_token(std::u16string const& s) -> token_t {
  if (std::isdigit(*s.begin()))
    return token_t::ARRAY;
  if (is_operator(s))
    return token_t::OPERATOR;
  return token_t::FUNCTION;
}

// TODO: enable vvv
// using item_variant = std::variant<int>;

// TODO: maybe rename to int tensor, or template
auto to_tensor(std::u16string str) -> tensor<int> {
  // TODO: need to fix this (need for the temp)
  auto const temp = str               //
                    | rv::split(' ')  //
                    | ranges::to<std::vector<std::u16string>>;
  auto const vec =
      temp                                                                  //
      | rv::transform([](auto i) { return std::stoi(utf8::utf16to8(i)); })  //
      | ranges::to<std::vector<int>>;
  return tensor{vec};
}

using token_variant = std::variant<std::u16string, tensor<int>>;

class token {
  token_t _type;
  token_variant _value;

 public:
  token(std::u16string str)
      : _type{u16_to_token(str)},
        _value{_type == token_t::ARRAY ? token_variant{to_tensor(str)}
                                       : token_variant{str}} {}

  token(tensor<int> arr) : _type{token_t::ARRAY}, _value{token_variant{arr}} {}

  [[nodiscard]] auto type() const { return _type; }
  [[nodiscard]] auto tensor_value() const -> tensor<int> {
    return std::get<tensor<int>>(_value);
  }

  // TODO: figure out whether we need to split this
  // (so that it isn't used for printing tensors)
  [[nodiscard]] auto u16string_value() const {
    if (_type == token_t::ARRAY)
      return utf8::utf8to16(tensor_value().string());
    return std::get<std::u16string>(_value);
  }
};
