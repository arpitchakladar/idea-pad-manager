#include "sys/utils/FileSystem.hpp"

#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <sys/stat.h>
#include <utility>

namespace ipm::sys::utils {

auto FileSystem::isRegularFile(std::string_view Path) -> bool {
  struct stat FileStat{};
  if (stat(std::string(Path).c_str(), &FileStat) != 0) {
    return false;
  }
  return S_ISREG(FileStat.st_mode);
}

auto FileSystem::readFile(std::string_view Path) -> std::optional<std::string> {
  auto File = std::ifstream(std::string(Path));
  if (!File.is_open()) {
    return std::nullopt;
  }

  auto Content = std::ostringstream();
  Content << File.rdbuf();
  auto Value = Content.str();

  if (!Value.empty() && Value.ends_with('\n')) {
    Value.pop_back();
  }

  return std::optional(std::move(Value));
}
} // namespace ipm::sys::utils
