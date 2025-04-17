#ifndef MIZU_MODULE_HPP
#define MIZU_MODULE_HPP

#include <string>

namespace mizu {
class Module {
public:
    virtual ~Module() = default;

    virtual std::string name() const = 0;
};
} // namespace mizu

#endif // MIZU_MODULE_HPP
