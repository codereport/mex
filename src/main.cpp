
#include <memory>  // for allocator, __shared_ptr_access
#include <optional>
#include <string>  // for char_traits, operator+, string, basic_string
#include <unordered_map>

#include "ftxui/component/captured_mouse.hpp"      // for ftxui
#include "ftxui/component/component.hpp"           // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/component_options.hpp"   // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"                  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"                      // for Ref

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/partial_sum.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>

#include <utf8.h>

#include <eval.hpp>
#include <tokenize.hpp>
#include <translate.hpp>

using namespace std::string_literals;
using namespace ftxui;

ftxui::Decorator const J_BLUE      = bgcolor(Color::RGB(2, 198, 245));
ftxui::Decorator const J_BLUE_DARK = bgcolor(Color::RGB(3, 155, 204));
ftxui::Decorator const APL_GREEN   = bgcolor(Color::RGB(36, 160, 72));
ftxui::Decorator const BQN_GREEN   = bgcolor(Color::RGB(43, 112, 102));

int main(int argc, const char* argv[]) {
    std::string expression;

    Component input = Input(&expression, "Enter expression...");
    auto component  = Container::Vertical({input});

    auto rpad = [](auto s, auto n) {
        auto const sz = utf8::utf8to16(s).size();
        if (sz > n) return s;
        return s + std::string(n - sz, ' ');
    };

    auto join = [](auto tokens) {
        std::u16string ret;
        for (auto const& tok : tokens) ret += tok.u16string_value();  // + utf8::utf8to16(" | ");
        return " " + utf8::utf16to8(ret);
    };

    auto format_error_msg = [](auto s) {
        auto const words = s | rv::split(' ');
        auto const psums = words | rv::transform(ranges::distance) | rv::partial_sum | rv::take_while(_lt(30));
        auto const n     = ranges::distance(psums);
        auto const l     = words | rv::take(n) | rv::join(' ') | ranges::to<std::string>;
        auto const r     = words | rv::drop(n) | rv::join(' ') | ranges::to<std::string>;
        return std::pair{l, r};
    };

    auto trace_window = [&] {
        auto trace = std::vector<std::vector<token>>{tokenize(expression)};
        auto e     = std::optional<error_msg>{};

        while (trace.back().size() > 1) {
            auto exp_tokens = eval_single_step(trace.back());
            if (not exp_tokens.has_value()) {
                e = exp_tokens.error().value();
                break;
            }
            trace.push_back(exp_tokens.value());
        }

        // TODO: this code is super hacky, needs to be cleaned up
        auto elems = Elements{};
        auto b     = false;
        auto add   = [&](auto tokens, std::string prefix) {
            elems.push_back(text(prefix + rpad(join(tokens), 35 - prefix.size())) |
                            bgcolor(b ? Color::Black : Color::GrayDark));
        };
        for (auto t : trace) {
            if (not t.empty() and std::ranges::count(t.back().u16string_value(), '\n')) {
                auto last = utf8::utf16to8(t.back().u16string_value());
                t.pop_back();

                // print first row
                auto first_row = last | rv::split('\n') | rv::take(1) | rv::join | ranges::to<std::string>;
                int const n    = utf8::utf8to16(join(t)).size() - 1;
                t.push_back(tokenize(first_row).front());
                add(t, "");

                // print subsequent rows (could require indentation)
                auto subsequent_rows = last | rv::split('\n') | rv::drop(1) | ranges::to<std::vector<std::string>>;
                auto indent          = std::string(n, ' ');
                for (auto row : subsequent_rows) {
                    auto new_trace = std::vector<token>{tokenize(row).front()};
                    add(new_trace, indent);
                }
            } else {
                add(t, "");
            }
            b = !b;
        }

        if (e.has_value()) {
            auto rest = e.value().string();
            auto line = ""s;
            while (not rest.empty()) {
                std::tie(line, rest) = format_error_msg(rest);
                elems.push_back(text("   " + line));
            }
        }

        auto content = vbox(elems);
        return window(text(L" Trace ") | bold, content);
    };

    auto train_tree_window = [&] {
        auto const blank_line = text("\n"s);

        auto elems = Elements(15, blank_line);
        elems[0]   = text(rpad(" TODO"s, 35));

        auto const content = vbox(elems);
        return window(text(L" Combinator Tree ") | bold, content);
    };

    auto renderer = Renderer(component, [&] {
        return vbox({vbox({hbox({hbox(text(" "), input->Render()) | flex,
                                 text(" M") | bold | color(Color::RGB(194, 52, 60)),
                                 text("e") | bold | color(Color::RGB(190, 45, 125)),
                                 text("X ") | bold | color(Color::RGB(220, 153, 50))}),
                           separator(),
                           text(translate::to<translate::ENGLISH>(expression)),
                           separator(),
                           hbox({text(" APL:" + translate::to<translate::APL>(expression)) | APL_GREEN | flex,
                                 separator(),
                                 text(" J:" + translate::to<translate::J>(expression)) | J_BLUE_DARK | flex,
                                 separator(),
                                 text(" BQN:" + translate::to<translate::BQN>(expression)) | BQN_GREEN | flex})}) |
                       border,
                     hbox({
                       trace_window() | flex,
                       train_tree_window() | flex,
                     })});
    });

    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);
}
