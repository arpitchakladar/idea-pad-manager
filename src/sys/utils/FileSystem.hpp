#pragma once

#include <dirent.h>
#include <optional>
#include <string>
#include <string_view>

namespace ipm::sys::utils {
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

class FileSystem {
public:
  static auto isRegularFile(std::string_view Path) -> bool;
  static auto readFile(std::string_view Path) -> std::optional<std::string>;
};
} // namespace ipm::sys::utils
