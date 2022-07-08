
#include <numeric>
#include <vector>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/reverse.hpp>

#include <utf8.h>

#include <tensor.hpp>

auto unary_iota(tensor<int> array) -> tensor<int> {
    if (array.rank() != 0) {
        // rank not supported with std::expected
    }
    auto const n = array.data().front();
    return tensor{rv::iota(1, n + 1) | ranges::to<std::vector>};
}

auto unary_reverse(tensor<int> array) -> tensor<int> {
    if (array.rank() > 1) {
        // rank not supported with std::expected
    }
    if (array.rank() == 0) { return array; }
    //   if (array.rank() == 1) {
    auto const copy = array.data();
    return tensor{copy | rv::reverse | ranges::to<std::vector>};
}
