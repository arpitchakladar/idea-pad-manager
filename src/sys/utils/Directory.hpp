#pragma once

#include <dirent.h>
#include <string_view>

namespace ipm::sys::utils {
class Directory {
public:
  explicit Directory(std::string_view Path);
  Directory(const Directory &) = delete;
  Directory &operator=(const Directory &) = delete;
  ~Directory();

  Directory(Directory &&Other) noexcept;
  Directory &operator=(Directory &&Other) noexcept;

  [[nodiscard]] auto get() -> DIR * { return m_Dir; }
  [[nodiscard]] auto isOpen() const -> bool { return m_Dir != nullptr; }

  auto release() -> DIR *;

private:
  DIR *m_Dir;
};
} // namespace ipm::sys::utils
