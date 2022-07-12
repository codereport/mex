
#pragma once

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip_with.hpp>

#include <utf8.h>

#include <fmt/core.h>

#include <combinators.hpp>
#include <error.hpp>
#include <token.hpp>

using namespace combinators;

auto is_binary_function(token function) -> bool {
    auto const& val = function.u16string_value();
    return val == utf8::utf8to16("+") or val == utf8::utf8to16("×") or val == utf8::utf8to16("÷") or
           val == utf8::utf8to16("-");
}

auto binary_generic_scalar(tensor<int> l, tensor<int> r, auto binop, std::string op) -> expected_tensor {
    if (l.shape() == r.shape()) {
        auto const data = rv::zip_with(binop, l.data(), r.data()) | ranges::to<std::vector>;
        return tensor{data, l.shape()};
    } else if (l.rank() == 0) {
        auto const val  = l.data().front();
        auto const data = r.data() | rv::transform(std::bind_front(binop, val)) | ranges::to<std::vector>;
        return tensor{data, r.shape()};
    } else if (r.rank() == 0) {
        // TODO:
        return make_tensor_error(
          error_type::NOT_IMPLEMENTED_YET,
          fmt::format("binary {} rank, left and right rank are {} & {}", op, l.rank(), r.rank()));
    }

    return make_tensor_error(
      error_type::DOMAIN,
      fmt::format("binary {} with left and right rank {} & {} not supported", op, l.rank(), r.rank()));
}

// clang-format off
auto binary_minus   (tensor<int> l, tensor<int> r) -> expected_tensor { return binary_generic_scalar(l, r, _sub_,  "-"); }
auto binary_multiply(tensor<int> l, tensor<int> r) -> expected_tensor { return binary_generic_scalar(l, r, _mul_,  "×"); }
auto binary_plus    (tensor<int> l, tensor<int> r) -> expected_tensor { return binary_generic_scalar(l, r, _plus_, "+"); }
// clang-format on
