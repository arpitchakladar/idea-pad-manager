#include "sys/utils/File.hpp"

#include <cctype>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <utility>

#include <spdlog/spdlog.h>

namespace ipm::sys::utils {

File::File(std::string Path)
  : m_Path(std::move(Path)) {}

auto File::isRegular() const -> bool {
  struct stat FileStat{};
  SPDLOG_INFO("{}", m_Path);
  if (stat(std::string(m_Path).c_str(), &FileStat) != 0) {
    return false;
  }
  return S_ISREG(FileStat.st_mode);
}

auto File::isWritable() const -> bool {
  return access(m_Path.c_str(), W_OK) == 0;
}

auto File::read() const -> std::optional<std::string> {
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

auto File::write(const std::string &Content) const -> bool {
  auto Stream = std::ofstream(m_Path, std::ios::out | std::ios::trunc);
  if (!Stream.is_open()) {
    SPDLOG_ERROR("Failed to open file for writing: {}", m_Path);
    return false;
  }
  Stream << Content;
  if (!Stream.good()) {
    SPDLOG_ERROR("Failed to write to file: {}", m_Path);
    return false;
  }
  return true;
}

auto File::getFieldName() const -> std::string {
  std::ostringstream Result;
  auto CapitalizeNext = true;

  const auto FileNameStartIndex = static_cast<uint>(m_Path.rfind('/')) + 1U;

  for (auto I = FileNameStartIndex, E = static_cast<uint>(m_Path.size()); I < E;
    ++I) {
    const auto C = m_Path[I];
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
