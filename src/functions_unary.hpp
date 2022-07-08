
#include <numeric>
#include <vector>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/reverse.hpp>

#include <utf8.h>

#include <tensor.hpp>
#include <token.hpp>

auto is_unary_function(token function) -> bool {
    auto const& val = function.u16string_value();
    return val == utf8::utf8to16("⍳") or val == utf8::utf8to16("⌽");
}

auto unary_iota(tensor<int> t) -> tensor<int> {
    if (t.rank() != 0) {
        // rank not supported with std::expected
    }
    auto const n = t.data().front();
    return tensor{rv::iota(1, n + 1) | ranges::to<std::vector>};
}

auto unary_reverse(tensor<int> t) -> tensor<int> {
    if (t.rank() > 1) {
        // rank not supported with std::expected
    }
    if (t.rank() == 0) return t;
    //   if (array.rank() == 1) {
    auto const copy = t.data();
    return tensor{copy | rv::reverse | ranges::to<std::vector>};
}
