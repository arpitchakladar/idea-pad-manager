#pragma once

#include <dirent.h>
#include <functional>
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

  [[nodiscard]] auto isOpen() const -> bool { return m_Dir != nullptr; }
  auto forEachEntry(const std::function<void(std::string_view)> &Function)
    -> void;

private:
  DIR *m_Dir;

  auto release() -> DIR *;
};
} // namespace ipm::sys::utils
