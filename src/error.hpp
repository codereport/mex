
#include <string>
#include <vector>

#include <include/tl/expected.hpp>

#include <tensor.hpp>
#include <token.hpp>

using namespace std::string_literals;

enum class error_type { NOT_IMPLEMENTED_YET, ARITY, RANK, SHAPE, DOMAIN };

auto to_string(error_type e) {
    switch (e) {
        case error_type::NOT_IMPLEMENTED_YET: return "not implemented yet"s;
        case error_type::ARITY: return "arity error"s;
        case error_type::RANK: return "rank error"s;
        case error_type::SHAPE: return "shape error"s;
        case error_type::DOMAIN: return "domain error"s;
    }
    return "unhandled error"s;
}

struct error_msg {
    error_type err;
    std::string msg;
    auto string() { return to_string(err) + ": " + msg; }
};

using error = tl::unexpected<error_msg>;

using expected_tensor = tl::expected<tensor<int>, error>;
using expected_tokens = tl::expected<std::vector<token>, error>;

auto make_tokens_error(error_type e, std::string msg) {
    return expected_tokens{tl::unexpect, error{error_msg{e, msg}}};
}