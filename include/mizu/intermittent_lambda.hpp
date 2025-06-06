#ifndef MIZU_INTERMITTENT_LAMBDA_HPP
#define MIZU_INTERMITTENT_LAMBDA_HPP

#include <chrono>
#include <functional>

namespace mizu {
template<typename T, typename... Args>
class IntermittentLambda {
public:
    using duration = std::chrono::steady_clock::duration;

    template<typename F>
    IntermittentLambda(duration interval, F f);

    T operator()(duration dt, Args... args);

private:
    duration interval_;
    duration acc_;

    std::function<T(Args...)> f_;
    T saved_result_;
};

template<typename T, typename... Args>
template<typename F>
IntermittentLambda<T, Args...>::IntermittentLambda(const duration interval, F f)
    : interval_(interval), acc_(duration::zero()), f_(f), saved_result_(T()) {}

template<typename T, typename... Args>
T IntermittentLambda<T, Args...>::operator()(duration dt, Args... args) {
    acc_ += dt;
    if (acc_ >= interval_) {
        saved_result_ = f_(args...);
        acc_ -= interval_;
    }
    return saved_result_;
}
} // namespace mizu

#endif // MIZU_INTERMITTENT_LAMBDA_HPP
