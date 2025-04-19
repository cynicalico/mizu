#ifndef MIZU_PAYLOADS_HPP
#define MIZU_PAYLOADS_HPP

namespace mizu {
struct PPreUpdate {};

struct PUpdate {
    double dt;
};

struct PPostUpdate {};

struct PPreDraw {};

struct PDraw {};

struct PPostDraw {};

struct PPresent {};
} // namespace mizu

#endif // MIZU_PAYLOADS_HPP
