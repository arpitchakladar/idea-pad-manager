#pragma once

#include <optional>
#include <string>

namespace ipm::sys::utils {
class File {
public:
  File(std::string Path);
  File(const File &) = default;
  File &operator=(const File &) = default;
  File(File &&) noexcept = default;
  File &operator=(File &&) noexcept = default;
  ~File() = default;

  [[nodiscard]] auto isRegular() const -> bool;
  [[nodiscard]] auto read() const -> std::optional<std::string>;
  [[nodiscard]] auto getFieldName() const -> std::string;

private:
  std::string m_Path;
};
} // namespace ipm::sys::utils
