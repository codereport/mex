
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

#include <utf8.h>

#include <combinators.hpp>
#include <token.hpp>

using namespace combinators;

auto is_binary_function(token function) -> bool {
    auto const& val = function.u16string_value();
    return val == utf8::utf8to16("+") or val == utf8::utf8to16("×") or val == utf8::utf8to16("÷");
}

auto binary_plus(tensor<int> l, tensor<int> r) -> tensor<int> {
    if (l.rank() != 0) {
        // TODO: rank not supported with std::expected
    }
    auto const val = l.data().front();
    return tensor{r.data() | rv::transform(_plus(val)) | ranges::to<std::vector>};
}
