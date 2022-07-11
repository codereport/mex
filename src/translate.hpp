
#include <algorithm>
#include <array>
#include <string>
#include <string_view>

#include "utf8.h"

#pragma once

namespace translate {

using lang_t = int32_t;

static constexpr lang_t APL     = 1;
static constexpr lang_t J       = 2;
static constexpr lang_t BQN     = 3;
static constexpr lang_t ENGLISH = 4;

// clang-format off
static constexpr std::
    array<std::tuple<char16_t, char16_t, std::string_view, char16_t, std::string_view>, 7>
        _map = {std::tuple{L'⍳',  L'⍳',  "i.",   L'↕', "iota"    },
                std::tuple{L'/',  L'/',  "/",    L'´', "reduce"  },
                std::tuple{L'\\', L'\\', "/\\",  L'`', "scan"    },
                std::tuple{L'⌈',  L'⌈',  ">.",   L'⌈', "max"     },
                std::tuple{L'⊢',  L'⊢',  "[",    L'⊢', "id"      },
                std::tuple{L'⌽',  L'⌽',  "|.",   L'⌽', "reverse" },
                std::tuple{L'×',  L'×',  "*",    L'×', "*"       }};
// clang-format on

auto find(char16_t c) {
    return std::find_if(_map.cbegin(), _map.cend(), [c](auto t) { return std::get<0>(t) == c; });
}

template <lang_t Lang>
auto to(std::string expr) -> std::string {
    auto u16 = utf8::utf8to16(expr);
    std::u16string ret;
    ret += ' ';
    for (auto c : u16) {
        auto it = find(c);
        if (it != _map.cend()) {
            if constexpr (Lang == translate::J or Lang == translate::ENGLISH) {
                ret += utf8::utf8to16(std::get<Lang>(*it));
            }
            if constexpr (Lang == translate::APL or Lang == translate::BQN) { ret += std::get<Lang>(*it); }
        } else {
            ret += static_cast<char>(c);
        }
        if constexpr (Lang == translate::ENGLISH) { ret += ' '; }
    }
    return utf8::utf16to8(ret);
}
};  // namespace translate
