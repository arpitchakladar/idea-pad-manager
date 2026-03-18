#pragma once

#include <optional>
#include <string>

namespace ipm::sys::utils {
class File {
public:
  File(std::string Path);
  File(const File &) = delete;
  File &operator=(const File &) = delete;
  File(File &&) noexcept = default;
  File &operator=(File &&) noexcept = default;
  ~File() = default;

  [[nodiscard]] auto isRegular() const -> bool;
  auto read() -> std::optional<std::string>;

private:
  std::string m_Path;
};
} // namespace ipm::sys::utils
