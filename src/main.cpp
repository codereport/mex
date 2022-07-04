
#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for char_traits, operator+, string, basic_string
#include <optional>

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref

using namespace std::string_literals;

int main(int argc, const char* argv[]) {
  using namespace ftxui;

  std::string expression;

  Component input = Input(&expression, "Enter expression...");

  auto component = Container::Vertical({input});

  auto trace_window = [&] {
    auto content = hbox({text(" TODO"s)});
    return window(text(L" Trace "), content);
  };

  auto train_tree_window = [&] {
    auto const blank_line = text("\n"s);
    auto content = vbox({
        text(" TODO"s),
        blank_line,
        blank_line,
        blank_line,
        blank_line,
        blank_line,
        blank_line,
        blank_line,
        blank_line,
        blank_line,
    });
    return window(text(L" Train Tree "), content);
  };

  auto renderer = Renderer(component, [&] {
    return vbox(
        {vbox({
             hbox({hbox(text(" "), input->Render()) | flex,
                   text(" M") | bold | color(Color::RGB(194, 52, 60)),
                   text("e") | bold | color(Color::RGB(190, 45, 125)),
                   text("X ") | bold | color(Color::RGB(220, 153, 50))}),
             separator(),
             text(" translation"),
         }) | border,
         hbox({
             trace_window() | flex,
             train_tree_window() | flex,
         })});
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}
