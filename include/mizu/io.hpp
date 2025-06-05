#ifndef MIZU_IO_HPP
#define MIZU_IO_HPP

#include <filesystem>
#include <optional>
#include <string>

namespace mizu {
std::optional<std::string> read_file(const std::filesystem::path &path);
} // namespace mizu

#endif // MIZU_IO_HPP
