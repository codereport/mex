
#include <iostream>
#include <string>

#include <boost/ut.hpp>

#include <eval.hpp>

using namespace boost::ut;

// TODO: replace iostream with fmt
template <typename T>
auto print_info(T t) {
    auto print = [](auto v) {
        for (auto e : v) std::cout << e << ' ';
        std::cout << '\n';
    };
    std::cout << to_string(t) << '\n';
    std::cout << "shape: ";
    print(t.shape());
    std::cout << "data: ";
    print(t.data());
}

auto main() -> int {
    "reverse"_test = [] {  //
        expect(eval("⌽1 2 3") == eval("3 2 1"));
        expect(eval("⌽1 2 3") == eval("⌽⌽3 2 1"));
    };

    return 0;
}
