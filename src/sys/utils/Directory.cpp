#include "sys/utils/Directory.hpp"

#include <string>

namespace ipm::sys::utils {
Directory::Directory(std::string_view Path)
  : m_Dir(opendir(std::string(Path).c_str())) {}

Directory::~Directory() {
  if (m_Dir != nullptr) {
    closedir(m_Dir);
  }
}

Directory::Directory(Directory &&Other) noexcept
  : m_Dir(Other.release()) {}

Directory &Directory::operator=(Directory &&Other) noexcept {
  if (this != &Other) {
    if (m_Dir != nullptr) {
      closedir(m_Dir);
    }
    m_Dir = Other.release();
  }
  return *this;
}

auto Directory::forEachChild(
  const std::function<void(std::string_view)> &Function) -> void {
  struct dirent *Entry = nullptr;
  while ((Entry = readdir(m_Dir)) != nullptr) {
    auto Filename = std::string_view(&Entry->d_name[0]);
    Function(Filename);
  }
}

auto Directory::release() -> DIR * {
  auto *Temp = m_Dir;
  m_Dir = nullptr;
  return Temp;
}
} // namespace ipm::sys::utils
