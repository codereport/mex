
#include <string>

#include <boost/ut.hpp>

#include <fmt/ranges.h>

#include <eval.hpp>

using namespace boost::ut;

template <typename T>
auto print_info(T t) {
    fmt::print("{}\n", to_string(t));
    fmt::print("shape: {}\n", t.shape());
    fmt::print("data:  {}\n", t.data());
}

auto main() -> int {
    "reverse"_test = [] {  //
        expect(eval("⌽1 2 3") == eval("3 2 1"));
        expect(eval("⌽1 2 3") == eval("⌽⌽3 2 1"));
    };

    return 0;
}
