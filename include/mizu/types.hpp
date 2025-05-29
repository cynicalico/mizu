#ifndef MIZU_TYPES_HPP
#define MIZU_TYPES_HPP

namespace mizu {
template<typename T>
struct Size2d {
    T w;
    T h;

    Size2d()
        : w(T()), h(T()) {}

    Size2d(T w, T h)
        : w(w), h(h) {}
};

template<typename T>
struct Pos2d {
    T x;
    T y;

    Pos2d()
        : x(T()), y(T()) {}

    Pos2d(T x, T y)
        : x(x), y(y) {}
};

template<typename T>
struct Rect2d {
    Pos2d<T> pos;
    Size2d<T> size;

    Rect2d()
        : pos(Pos2d<T>()), size(Size2d<T>()) {}

    Rect2d(Pos2d<T> pos, Size2d<T> size)
        : pos(pos), size(size) {}
};
} // namespace mizu

#endif // MIZU_TYPES_HPP
