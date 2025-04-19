#ifndef MIZU_ENUM_CLASS_BITOPS_HPP
#define MIZU_ENUM_CLASS_BITOPS_HPP

#include <ranges>
#include <type_traits>

/* Enum class iteration
 * Requirements:
 * 1. Enums must have a numeric underlying type
 * 2. There must be no gaps in the underlying values (i.e., 1, 2, 3 is fine, 1, 2, 4 is not)
 */

constexpr inline auto enum_range = [](auto front, auto back) {
    return std::views::iota(std::to_underlying(front), std::to_underlying(back) + 1) |
           std::views::transform([](auto e) { return decltype(front)(e); });
};

/* Enum class bitmask operations
 */

template<typename Enum>
struct MizuEnableBitops {
    static constexpr bool enable = false;
};

template<typename Enum>
std::enable_if_t<MizuEnableBitops<Enum>::enable, Enum> operator|(Enum lhs, Enum rhs) {
    using underlying = std::underlying_type_t<Enum>;
    return static_cast<Enum>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename Enum>
std::enable_if_t<MizuEnableBitops<Enum>::enable, Enum> &operator|=(Enum &lhs, Enum rhs) {
    using underlying = std::underlying_type_t<Enum>;
    return lhs = static_cast<Enum>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename Enum>
std::enable_if_t<MizuEnableBitops<Enum>::enable, Enum> operator&(Enum lhs, Enum rhs) {
    using underlying = std::underlying_type_t<Enum>;
    return static_cast<Enum>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename Enum>
std::enable_if_t<MizuEnableBitops<Enum>::enable, Enum> &operator&=(Enum &lhs, Enum rhs) {
    using underlying = std::underlying_type_t<Enum>;
    return lhs = static_cast<Enum>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename Enum>
std::enable_if_t<MizuEnableBitops<Enum>::enable, std::underlying_type_t<Enum>> unwrap(Enum e) {
    using underlying = std::underlying_type_t<Enum>;
    return static_cast<underlying>(e);
}

template<typename Enum>
std::enable_if_t<MizuEnableBitops<Enum>::enable, bool> is_flag_set(Enum e, Enum mask) {
    return (e & mask) == mask;
}

#define ENUM_CLASS_ENABLE_BITOPS(x)                                                                                    \
    template<>                                                                                                         \
    struct MizuEnableBitops<x> {                                                                                       \
        static constexpr bool enable = true;                                                                           \
    }

#endif // MIZU_ENUM_CLASS_BITOPS_HPP
