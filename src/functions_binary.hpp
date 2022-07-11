
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

#include <utf8.h>

#include <combinators.hpp>
#include <error.hpp>
#include <token.hpp>

using namespace combinators;

auto is_binary_function(token function) -> bool {
    auto const& val = function.u16string_value();
    return val == utf8::utf8to16("+") or val == utf8::utf8to16("×") or val == utf8::utf8to16("÷") or
           val == utf8::utf8to16("-");
}

// TODO: binary_generic_scalar
auto binary_minus(tensor<int> l, tensor<int> r) -> expected_tensor {
    if (l.rank() != 0) {
        return make_tensor_error(
          error_type::NOT_IMPLEMENTED_YET,
          "binary - with left argument with rank != 0, rank currently " + std::to_string(l.rank()));
    }
    auto const val = l.data().front();
    return tensor{r.data() | rv::transform(_sub(val)) | ranges::to<std::vector>};
}

auto binary_multiply(tensor<int> l, tensor<int> r) -> expected_tensor {
    if (l.rank() != 0) {
        return make_tensor_error(
          error_type::NOT_IMPLEMENTED_YET,
          "binary × with left argument with rank != 0, rank currently " + std::to_string(l.rank()));
    }
    auto const val = l.data().front();
    return tensor{r.data() | rv::transform(_mul(val)) | ranges::to<std::vector>};
}

auto binary_plus(tensor<int> l, tensor<int> r) -> expected_tensor {
    if (l.rank() != 0) {
        return make_tensor_error(
          error_type::NOT_IMPLEMENTED_YET,
          "binary + with left argument with rank != 0, rank currently " + std::to_string(l.rank()));
    }
    auto const val = l.data().front();
    return tensor{r.data() | rv::transform(_plus(val)) | ranges::to<std::vector>};
}
