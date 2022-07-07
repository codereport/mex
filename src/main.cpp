
#include <memory>  // for allocator, __shared_ptr_access
#include <optional>
#include <string>  // for char_traits, operator+, string, basic_string
#include <unordered_map>

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

#include <utf8.h>

#include <eval.hpp>
#include <tokenize.hpp>
#include <translate.hpp>

using namespace std::string_literals;

int main(int argc, const char* argv[]) {
  using namespace ftxui;

  std::string expression;

  Component input = Input(&expression, "Enter expression...");

  auto component = Container::Vertical({input});

  auto rpad = [](auto s, auto n) {
    auto const sz = utf8::utf8to16(s).size();
    if (sz > n)
      return s;
    return s + std::string(n - sz, ' ');
  };

  auto join = [](auto tokens) {
    std::u16string ret;
    for (auto const& tok : tokens)
      ret += tok.u16string_value();
    return " " + utf8::utf16to8(ret);
  };

  auto trace_window = [&] {
    auto trace = std::vector<std::vector<token>>{tokenize(expression)};

    while (trace.back().size() > 1) {
      auto tokens = eval_single_step(trace.back());
      trace.push_back(tokens);
    }

    auto elems =
        trace                                                             //
        | rv::transform([&](auto t) { return text(rpad(join(t), 30)); })  //
        | ranges::to<Elements>;

    auto content = vbox(elems);
    return window(text(L" Trace "), content);
  };

  auto train_tree_window = [&] {
    auto const blank_line = text("\n"s);

    auto elems = Elements(15, blank_line);
    elems[0] = text(rpad(" TODO"s, 30));

    auto content = vbox(elems);
    return window(text(L" Combinator Tree "), content);
  };

  auto renderer = Renderer(component, [&] {
    return vbox(
        {vbox({hbox({hbox(text(" "), input->Render()) | flex,
                     text(" M") | bold | color(Color::RGB(194, 52, 60)),
                     text("e") | bold | color(Color::RGB(190, 45, 125)),
                     text("X ") | bold | color(Color::RGB(220, 153, 50))}),
               separator(), text(translate::to<translate::ENGLISH>(expression)),
               separator(),
               hbox({text(" APL:" + translate::to<translate::APL>(expression)) |
                         flex,
                     separator(),
                     text(" J:" + translate::to<translate::J>(expression)) |
                         flex,
                     separator(),
                     text(" BQN:" + translate::to<translate::BQN>(expression)) |
                         flex})}) |
             border,
         hbox({
             trace_window() | flex,
             train_tree_window() | flex,
         })});
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}
