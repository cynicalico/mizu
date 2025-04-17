#include "mizu/id_mgr.hpp"
#include "mizu/log.hpp"

namespace mizu {
IdMgr::IdMgr() {
    // id 0 is invalid, fill the spot with something
    id_to_name_.emplace_back("");
}

std::string IdMgr::name_from_id(std::size_t id) const {
    if (id < id_to_name_.size())
        return id_to_name_[id];
    return "";
}

std::size_t IdMgr::get_(std::string name) {
    auto it = name_to_id_.find(name);
    if (it == name_to_id_.end()) {
        it = name_to_id_.emplace_hint(it, name, id_to_name_.size());
        id_to_name_.emplace_back(name);
    }

    return it->second;
}
} // namespace mizu
