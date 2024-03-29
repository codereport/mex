
#pragma once

#include <algorithm>
#include <array>
#include <string>
#include <string_view>

#include <combinators.hpp>

#include <utf8.h>

using namespace combinators;

namespace translate {

using lang_t = int32_t;

static constexpr lang_t APL     = 1;
static constexpr lang_t J       = 2;
static constexpr lang_t BQN     = 3;
static constexpr lang_t ENGLISH = 4;

// clang-format off
static constexpr std::
    array<std::tuple<char32_t, char16_t, std::string_view, char16_t, std::string_view>, 9>
        _map = {std::tuple{L'⍳',  L'⍳',  "i.",   L'↕', "iota"    },
                std::tuple{L'/',  L'/',  "/",    L'´', "reduce"  },
                std::tuple{L'\\', L'\\', "/\\",  L'`', "scan"    },
                std::tuple{L'⌈',  L'⌈',  ">.",   L'⌈', "max"     },
                std::tuple{L'⊢',  L'⊢',  "[",    L'⊢', "id"      },
                std::tuple{L'⌽',  L'⌽',  "|.",   L'⌽', "reverse" },
                std::tuple{u'×',  L'×',  "*",    L'×', "*"       },
                std::tuple{U'⍴',  L'⍴',  "$",    L'⥊', "reshape" },
                std::tuple{U'#',  L'≢',  "#",    L'≠', "length" }};
// clang-format on

auto find(char32_t c) {
    return std::find_if(_map.cbegin(), _map.cend(), [c](auto t) { return std::get<0>(t) == c; });
}

template <lang_t Lang>
auto to(std::string expr) -> std::string {
    auto u16 = utf8::utf8to32(expr);
    std::u32string ret;
    ret += ' ';
    auto prev = '?';
    for (auto c : u16) {
        if constexpr (Lang == translate::ENGLISH) {
            if (_psi(_and_, ::isdigit)(c, prev)) { ret.pop_back(); }
        }
        auto it = find(c);
        if (it != _map.cend()) {
            if constexpr (Lang == translate::J or Lang == translate::ENGLISH) {
                ret += utf8::utf8to32(std::get<Lang>(*it));
            }
            if constexpr (Lang == translate::APL or Lang == translate::BQN) { ret += std::get<Lang>(*it); }
        } else {
            if (Lang == translate::ENGLISH and c == ' ') {
                prev = ' ';
                continue;
            }
            ret += static_cast<char>(c);
        }
        if constexpr (Lang == translate::ENGLISH) { ret += ' '; }
        prev = c;
    }
    return utf8::utf32to8(ret);
}
};  // namespace translate
