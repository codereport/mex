
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
    "unary functions"_test = [] {
        "iota"_test = [] {
            expect(eval("⍳3") == eval("1 2 3"));
            expect(eval("⍳10") == eval("1 2 3 4 5 6 7 8 9 10"));
        };

        "length"_test = [] {
            expect(eval("#⍳3") == eval("3"));
            expect(eval("#1 2 3 4 5") == eval("5"));
            expect(eval("#10") == eval("1"));
            expect(eval("#⍳0") == eval("0"));
        };

        "reverse"_test = [] {
            expect(eval("⌽1 2 3") == eval("3 2 1"));
            expect(eval("⌽1 2 3") == eval("⌽⌽3 2 1"));
        };
    };

    "binary functions"_test = [] {
        "minus"_test = [] {
            expect(eval("10-1 2 3") == eval("9 8 7"));
            expect(eval("5-1") == eval("4"));
            expect(eval("1 2 3-1 2 3") == eval("0 0 0"));
        };

        "multiplies"_test = [] {
            expect(eval("1×1 2 3") == eval("1 2 3"));
            expect(eval("2×1 2 3") == eval("2 4 6"));
            expect(eval("10×1 2 3") == eval("10 20 30"));
            expect(eval("1 2 3×1 2 3") == eval("1 4 9"));
        };

        "plus"_test = [] {
            expect(eval("1+1 2 3") == eval("2 3 4"));
            expect(eval("10+1 2 3") == eval("11 12 13"));
            expect(eval("1 2 3+1 2 3") == eval("2 4 6"));
        };
    };

    return 0;
}
