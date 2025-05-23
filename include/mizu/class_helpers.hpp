#ifndef MIZU_CLASS_HELPERS_HPP
#define MIZU_CLASS_HELPERS_HPP

#define COPY_CONSTRUCTOR(x) x(const x &other)
#define COPY_ASSIGN_OP(x) x &operator=(const x &other)

#define COPY_CONSTRUCTOR_IMPL(x) x::x(const x &other)
#define COPY_ASSIGN_OP_IMPL(x) x &x::operator=(const x &other)

#define NO_COPY(x)                                                                                                     \
    COPY_CONSTRUCTOR(x) = delete;                                                                                      \
    COPY_ASSIGN_OP(x) = delete;

#define MOVE_CONSTRUCTOR(x) x(x &&other) noexcept
#define MOVE_CONSTRUCTOR_IMPL(x) x::x(x &&other) noexcept

#define MOVE_ASSIGN_OP(x) x &operator=(x &&other) noexcept
#define MOVE_ASSIGN_OP_IMPL(x) x &x::operator=(x &&other) noexcept

#define NO_MOVE(x)                                                                                                     \
    MOVE_CONSTRUCTOR(x) = delete;                                                                                      \
    MOVE_ASSIGN_OP(x) = delete;

#endif // MIZU_CLASS_HELPERS_HPP
