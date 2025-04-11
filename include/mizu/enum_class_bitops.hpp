#ifndef MIZU_ENUM_CLASS_BITOPS_HPP
#define MIZU_ENUM_CLASS_BITOPS_HPP

#include <type_traits>

namespace mizu {
template<typename Enum>
struct EnableBitops {
    static constexpr bool enable = false;
};

template<typename Enum>
std::enable_if_t<EnableBitops<Enum>::enable, Enum> operator|(Enum lhs, Enum rhs) {
    using underlying = std::underlying_type_t<Enum>;
    return static_cast<Enum>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename Enum>
std::enable_if_t<EnableBitops<Enum>::enable, Enum> &operator|=(Enum &lhs, Enum rhs) {
    using underlying = std::underlying_type_t<Enum>;
    return lhs = static_cast<Enum>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename Enum>
std::enable_if_t<EnableBitops<Enum>::enable, Enum> operator&(Enum lhs, Enum rhs) {
    using underlying = std::underlying_type_t<Enum>;
    return static_cast<Enum>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename Enum>
std::enable_if_t<EnableBitops<Enum>::enable, Enum> &operator&=(Enum &lhs, Enum rhs) {
    using underlying = std::underlying_type_t<Enum>;
    return lhs = static_cast<Enum>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename Enum>
std::underlying_type_t<Enum> unwrap(Enum e) {
    using underlying = std::underlying_type_t<Enum>;
    return static_cast<underlying>(e);
}

template<typename Enum>
bool is_flag_set(Enum e, Enum mask) {
    return (e & mask) == mask;
}
} // namespace mizu

#define ENUM_CLASS_ENABLE_BITOPS(x)                                                                                    \
    template<>                                                                                                         \
    struct mizu::EnableBitops<x> {                                                                                     \
        static constexpr bool enable = true;                                                                           \
    }

#endif // MIZU_ENUM_CLASS_BITOPS_HPP
