#include "sys/AboutSystem.hpp"

#include <cctype>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <sys/stat.h>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#include "ui/pages/Page.hpp"

namespace ipm::sys {
namespace {

auto snakeToTitleCase(std::string_view Snake) -> std::string {
  std::ostringstream Result;
  auto CapitalizeNext = true;

  for (unsigned char const C : Snake) {
    if (C == '_') {
      CapitalizeNext = true;
      Result << ' ';
    } else if (CapitalizeNext) {
      Result << static_cast<char>(std::toupper(C));
      CapitalizeNext = false;
    } else {
      Result << C;
    }
  }

  return Result.str();
}

auto readFile(std::string_view Path) -> std::string {
  auto File = std::ifstream(std::string(Path));
  if (!File.is_open()) {
    return "ROOT PERMISSION REQUIRED";
  }

  auto Content = std::ostringstream();
  Content << File.rdbuf();
  auto Value = Content.str();

  if (!Value.empty() && Value.ends_with('\n')) {
    Value.pop_back();
  }

  return Value;
}

class DirectoryHandle {
public:
  explicit DirectoryHandle(std::string_view Path)
    : m_Dir(opendir(std::string(Path).c_str())) {}

  ~DirectoryHandle() {
    if (m_Dir != nullptr) {
      closedir(m_Dir);
    }
  }

  DirectoryHandle(const DirectoryHandle &) = delete;
  DirectoryHandle &operator=(const DirectoryHandle &) = delete;

  DirectoryHandle(DirectoryHandle &&Other) noexcept
    : m_Dir(Other.release()) {}
  DirectoryHandle &operator=(DirectoryHandle &&Other) noexcept {
    if (this != &Other) {
      if (m_Dir != nullptr) {
        closedir(m_Dir);
      }
      m_Dir = Other.release();
    }
    return *this;
  }

  [[nodiscard]] DIR *get() const { return m_Dir; }
  [[nodiscard]] bool isOpen() const { return m_Dir != nullptr; }

  auto release() -> DIR * {
    auto *Temp = m_Dir;
    m_Dir = nullptr;
    return Temp;
  }

private:
  DIR *m_Dir;
};
auto isRegularFile(std::string_view Path) -> bool {
  struct stat FileStat{};
  if (stat(std::string(Path).c_str(), &FileStat) != 0) {
    return false;
  }
  return S_ISREG(FileStat.st_mode);
}

} // namespace

auto AboutSystem::aboutSystemInfo()
  -> std::vector<std::variant<ui::pages::RowStatic,
    ui::pages::RowDynamic,
    ui::pages::RowCustom>> {
  std::vector<std::variant<ui::pages::RowStatic,
    ui::pages::RowDynamic,
    ui::pages::RowCustom>>
    Rows;

  DirectoryHandle const DirHandle("/sys/class/dmi/id/");
  if (!DirHandle.isOpen()) {
    return Rows;
  }

  struct dirent *Entry = nullptr;
  while ((Entry = readdir(DirHandle.get())) != nullptr) {
    const auto Filename = std::string_view(Entry->d_name);

    if (Filename == "." || Filename == "..") {
      continue;
    }

    std::string const Path = "/sys/class/dmi/id/" + std::string(Filename);

    if (!isRegularFile(Path)) {
      continue;
    }

    std::string Value = readFile(Path);
    std::string TitleCase = snakeToTitleCase(Filename);

    Rows.emplace_back(std::make_tuple(std::move(TitleCase), std::move(Value)));
  }

  return Rows;
}

} // namespace ipm::sys
