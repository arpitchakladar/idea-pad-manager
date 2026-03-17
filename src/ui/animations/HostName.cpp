#include "ui/animations/HostName.hpp"

#include <algorithm>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

#include "ui/animations/CanvasAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {
namespace {
// clang-format off
// NOLINTBEGIN
const std::vector<std::vector<std::string>> g_BigFont = {
  // 0 - A
  {
    "           ",
    "     /\\    ",
    "    /  \\   ",
    "   / /\\ \\  ",
    "  / ____ \\ ",
    " /_/    \\_\\",
    "           ",
    "           "
  },
  // 1 - B
  {
      "  ____  ",
      " |  _ \\ ",
      " | |_) |",
      " |  _ < ",
      " | |_) |",
      " |____/ ",
      "        ",
      "        "
  },
  // 2 - C
  {
      "   _____ ",
      "  / ____|",
      " | | $   ",
      " | | $   ",
      " | |____ ",
      "  \\_____|",
      "         ",
      "         "
  },
  // 3 - D
  {
    "  _____  ",
    " |  __ \\ ",
    " | |  | |",
    " | |  | |",
    " | |__| |",
    " |_____/ ",
    "         ",
    "         "
  },
  // 4 - E
  {
    "  ______ ",
    " |  ____|",
    " | |__   ",
    " |  __|  ",
    " | |____ ",
    " |______|",
    "         ",
    "         "
  },
  // 5 - F
  {
    "  ______ ",
    " |  ____|",
    " | |__   ",
    " |  __|  ",
    " | |     ",
    " |_|     ",
    "         ",
    "         "
  },
  // 6 - G
  {
    "   _____ ",
    "  / ____|",
    " | |  __ ",
    " | | |_ |",
    " | |__| |",
    "  \\_____|",
    "         ",
    "         "
  },
  // 7 - H
  {
    "  _    _ ",
    " | |  | |",
    " | |__| |",
    " |  __  |",
    " | |  | |",
    " |_|  |_|",
    "         ",
    "         "
  },
  // 8 - I
  {
    "  _____ ",
    " |_   _|",
    "   | |  ",
    "   | |  ",
    "  _| |_ ",
    " |_____|",
    "        ",
    "        "
  },
  // 9 - J
  {
    "        _ ",
    "       | |",
    "       | |",
    "   _   | |",
    "  | |__| |",
    "   \\____/ ",
    "         ",
    "         "
  },
  // 10 - K
  {
    "  _  __",
    " | |/ /",
    " | ' / ",
    " |  <  ",
    " | . \\ ",
    " |_|\\_\\",
    "       ",
    "       "
  },
  // 11 - L
  {
    "  _      ",
    " | |     ",
    " | |     ",
    " | |     ",
    " | |____ ",
    " |______|",
    "         ",
    "         "
  },
  // 12 - M
  {
    "  __  __ ",
    " |  \\/  |",
    " | \\  / |",
    " | |\\/| |",
    " | |  | |",
    " |_|  |_|",
    "         ",
    "         "
  },
  // 13 - N
  {
    "  _   _ ",
    " | \\ | |",
    " |  \\| |",
    " | . ` |",
    " | |\\  |",
    " |_| \\_|",
    "        ",
    "        "
  },
  // 14 - O
  {
    "   ____  ",
    "  / __ \\ ",
    " | |  | |",
    " | |  | |",
    " | |__| |",
    "  \\____/ ",
    "         ",
    "         "
  },
  // 15 - P
  {
    "  _____  ",
    " |  __ \\ ",
    " | |__) |",
    " |  ___/ ",
    " | |     ",
    " |_|     ",
    "         ",
    "         "
  },
  // 16 - Q
  {
    "   ____  ",
    "  / __ \\ ",
    " | |  | |",
    " | |  | |",
    " | |__| |",
    "  \\___\\_\\",
    "         ",
    "         "
  },
  // 17 - R
  {
    "  _____  ",
    " |  __ \\ ",
    " | |__) |",
    " |  _  / ",
    " | | \\ \\ ",
    " |_|  \\_\\",
    "         ",
    "         "
  },
  // 18 - S
  {
    "   _____ ",
    "  / ____|",
    " | (___  ",
    "  \\___ \\ ",
    "  ____) |",
    " |_____/ ",
    "         ",
    "         "
  },
  // 19 - T
  {
    "  _______ ",
    " |__   __|",
    "    | |   ",
    "    | |   ",
    "    | |   ",
    "    |_|   ",
    "          ",
    "          "
  },
  // 20 - U
  {
    "  _    _ ",
    " | |  | |",
    " | |  | |",
    " | |  | |",
    " | |__| |",
    "  \\____/ ",
    "         ",
    "         "
  },
  // 21 - V
  {
    " __      __",
    " \\ \\    / /",
    "  \\ \\  / / ",
    "   \\ \\/ /  ",
    "    \\  /   ",
    "     \\/    ",
    "           ",
    "           "
  },
  // 22 - W
  {
    " __          __",
    " \\ \\        / /",
    "  \\ \\  /\\  / / ",
    "   \\ \\/  \\/ /  ",
    "    \\  /\\  /   ",
    "     \\/  \\/    ",
    "               ",
    "               "
  },
  // 23 - X
  {
    " __   __",
    " \\ \\ / /",
    "  \\ V / ",
    "   > <  ",
    "  / . \\ ",
    " /_/ \\_\\",
    "        ",
    "        "
  },
  // 24 - Y
  {
    " __     __",
    " \\ \\   / /",
    "  \\ \\_/ / ",
    "   \\   /  ",
    "    | |   ",
    "    |_|   ",
    "          ",
    "          "
  },
  // 25 - Z
  {
    "  ______",
    " |___  /",
    "    $/ / ",
    "    / /  ",
    "  / /__ ",
    " /_____|",
    "        ",
    "        "
  }
};
// NOLINTEND
// clang-format on

auto isValidBigText(const std::string &Text) -> bool {
  return !std::ranges::any_of(Text, [](auto Char) -> bool {
    const auto UpperCaseChar = std::toupper(Char);
    return UpperCaseChar < 'A' || UpperCaseChar > 'Z';
  });
}

auto bigTextWidth(const std::string &Text) -> uint {
  return std::ranges::fold_left(Text,
           0U,
           [](auto TotalWidth, auto Char) -> uint {
             const auto CharacterIndex =
               static_cast<uint>(std::toupper(Char) - 'A');
             const auto &CurrentCharacter = g_BigFont[CharacterIndex];
             const auto CurrentCharacterWidth =
               static_cast<uint>(std::ranges::max_element(
                 CurrentCharacter.begin(),
                 CurrentCharacter.end(),
                 [](const auto &Line1, const auto &Line2) -> bool {
                   return Line1.size() < Line2.size();
                 })->size());
             return TotalWidth + CurrentCharacterWidth;
           }) *
    2U;
}
} // namespace

auto HostName::resize(utils::CanvasSize CanvasSize) -> void {
  CanvasAnimation::resize(CanvasSize);
}

auto HostName::update() -> void {}

auto HostName::drawCanvas() const -> utils::CustomCanvas {
  auto Canvas = utils::CustomCanvas(canvasSize());

  drawBigTextCenter(Canvas, "LINUX");

  return Canvas;
}

auto HostName::drawBigTextCenter(
  utils::CustomCanvas &Canvas, const std::string &Text) const -> void {
  if (!isValidBigText(Text)) {
    // TODO: Add error handling
    return;
  }
  const auto CenterX = canvasSize().Width / 2U;
  const auto CenterY = canvasSize().Height / 2U;
  const auto BigTextWidth = bigTextWidth(Text);
  const auto BigTextHeight = g_BigFont[0].size();
  const auto BigTextHalfWidth = BigTextWidth / 2U;
  const auto BigTextHalfHeight =
    static_cast<uint>(BigTextHeight * utils::CanvasSize::k_CharacterHeight) /
    2U;
  const auto StartX = CenterX - BigTextHalfWidth;
  auto Y = CenterY - BigTextHalfHeight;
  for (auto I = 0U; I < BigTextHeight; ++I) {
    auto X = StartX;
    for (char Char : Text) {
      const auto CharacterIndex = static_cast<uint>(std::toupper(Char) - 'A');
      const auto &CurrentCharacterLine = g_BigFont[CharacterIndex][I];
      Canvas.DrawText(static_cast<int>(X),
        static_cast<int>(Y),
        CurrentCharacterLine,
        ftxui::Color::Red);
      X += CurrentCharacterLine.size() * utils::CanvasSize::k_CharacterWidth;
    }
    Y += utils::CanvasSize::k_CharacterHeight;
  }
}
} // namespace ipm::ui::animations
