#ifndef MIZU_ID_MGR_HPP
#define MIZU_ID_MGR_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include "mizu/module.hpp"

namespace mizu {
class IdMgr {
public:
    IdMgr();

    template<typename T>
        requires std::derived_from<T, Module>
    std::size_t get(const T &module);

    std::string name_from_id(std::size_t id) const;

private:
    std::unordered_map<std::string, std::size_t> name_to_id_{};
    std::vector<std::string> id_to_name_{};

    std::size_t get_(std::string name);
};

template<typename T>
    requires std::derived_from<T, Module>
std::size_t IdMgr::get(const T &module) {
    return get_(module.name());
}

} // namespace mizu

#endif // MIZU_ID_MGR_HPP
