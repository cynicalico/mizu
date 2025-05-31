#ifndef MIZU_IS_ANY_OF_HPP
#define MIZU_IS_ANY_OF_HPP

#include <concepts>

namespace mizu {
template<typename T, typename... U>
concept IsAnyOf = (std::same_as<T, U> || ...);
} // namespace mizu

#endif // MIZU_IS_ANY_OF_HPP
