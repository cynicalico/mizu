#include "mizu/io.hpp"
#include <fstream>
#include "mizu/log.hpp"

namespace mizu {
std::optional<std::string> read_file(const std::filesystem::path &path) {
    const auto size = file_size(path);
    std::string content(size, '\0');

    std::ifstream in(path);
    if (!in.is_open()) {
        MIZU_LOG_ERROR("Failed to open file: '{}'", path);
        return std::nullopt;
    }
    in.read(&content[0], size);

    return content;
}
} // namespace mizu
