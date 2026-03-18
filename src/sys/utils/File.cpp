#include "sys/utils/File.hpp"

#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <utility>

namespace ipm::sys::utils {

File::File(std::string Path)
  : m_Path(std::move(Path)) {}

auto File::isRegular() const -> bool {
  struct stat FileStat{};
  if (stat(std::string(m_Path).c_str(), &FileStat) != 0) {
    return false;
  }
  return S_ISREG(FileStat.st_mode);
}

auto File::read() -> std::optional<std::string> {
  auto File = std::ifstream(m_Path);
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

auto File::getFieldName() -> std::string {
  std::ostringstream Result;
  auto CapitalizeNext = true;

  for (unsigned char const C : m_Path) {
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
} // namespace ipm::sys::utils
