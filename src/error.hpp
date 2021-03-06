
#pragma once

#include <string>
#include <vector>

#include <tl/expected.hpp>

#include <tensor.hpp>
#include <token.hpp>

using namespace std::string_literals;

enum class error_type { NOT_IMPLEMENTED_YET, ARITY, RANK, SHAPE, DOMAIN, SYNTAX };

// clang-format off
auto to_string(error_type e) {
    switch (e) {
        case error_type::NOT_IMPLEMENTED_YET: return "not implemented yet"s;
        case error_type::ARITY:               return "arity error"s;
        case error_type::RANK:                return "rank error"s;
        case error_type::SHAPE:               return "shape error"s;
        case error_type::DOMAIN:              return "domain error"s;
        case error_type::SYNTAX:              return "syntax error"s;
    }
    return "unhandled error"s;
}
// clang-format on

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

auto make_tokens_error(error_msg e) { return expected_tokens{tl::unexpect, error{e}}; }

auto make_tensor_error(error_type e, std::string msg) {
    return expected_tensor{tl::unexpect, error{error_msg{e, msg}}};
}
