#ifndef MIZU_TYPES_HPP
#define MIZU_TYPES_HPP

namespace mizu {
template<typename T>
struct size2d {
    T w;
    T h;

    size2d() : w(T()), h(T()) {}

    size2d(T w, T h) : w(w), h(h) {}
};

template<typename T>
struct pos2d {
    T x;
    T y;

    pos2d() : x(T()), y(T()) {}

    pos2d(T x, T y) : x(x), y(y) {}
};

template<typename T>
struct rect2d {
    pos2d<T> pos;
    size2d<T> size;

    rect2d() : pos(pos2d<T>()), size(size2d<T>()) {}

    rect2d(pos2d<T> pos, size2d<T> size) : pos(pos), size(size) {}
};
} // namespace mizu

#endif // MIZU_TYPES_HPP
