#ifndef MIZU_TIME_HPP
#define MIZU_TIME_HPP

#include <chrono>
#include <deque>
#include <functional>
#include "mizu/averagers.hpp"

namespace mizu {
template<typename Clock = std::chrono::steady_clock>
class Ticker {
public:
    using time_point = std::chrono::time_point<Clock>;
    using duration = typename Clock::duration;

    explicit Ticker(duration interval = duration::zero());

    void reset();

    std::uint64_t tick();

    duration dt() const { return dt_; }

    duration elapsed() const { return last_ - start_; }

private:
    time_point start_;
    time_point last_;
    duration interval_;
    duration dt_;
    duration acc_;
};

template<typename Clock>
Ticker<Clock>::Ticker(duration interval)
    : start_(Clock::now()), last_(start_), interval_(interval), dt_(0), acc_(0) {}

template<typename Clock>
void Ticker<Clock>::reset() {
    start_ = Clock::now();
    last_ = start_;
    dt_ = duration::zero();
    acc_ = duration::zero();
}

template<typename Clock>
std::uint64_t Ticker<Clock>::tick() {
    const auto now = Clock::now();
    dt_ = now - last_;
    last_ = now;

    std::uint64_t tick_count = 0;
    if (interval_ != duration::zero()) {
        acc_ += dt_;
        while (acc_ >= interval_) {
            acc_ -= interval_;
            tick_count++;
        }
    }

    return tick_count;
}

template<typename Clock = std::chrono::steady_clock>
class FrameCounter {
public:
    using time_point = typename Ticker<Clock>::time_point;
    using duration = typename Ticker<Clock>::duration;

    FrameCounter();

    void update();

    double fps() const;

    duration dt() const;

private:
    std::deque<time_point> timestamps_{};
    Ticker<Clock> ticker_;
    EMA averager_;
};

template<typename Clock>
FrameCounter<Clock>::FrameCounter()
    : ticker_(std::chrono::milliseconds(1000)), averager_(1.0) {}

template<typename Clock>
void FrameCounter<Clock>::update() {
    timestamps_.push_back(Clock::now());

    while (timestamps_.size() > 2 && timestamps_.back() - timestamps_.front() > std::chrono::seconds(1))
        timestamps_.pop_front();

    averager_.update(static_cast<double>(timestamps_.size()));
    if (ticker_.tick() > 0)
        averager_.alpha = 2.0 / (1.0 + static_cast<double>(timestamps_.size()));
}

template<typename Clock>
double FrameCounter<Clock>::fps() const {
    return averager_.value();
}

template<typename Clock>
typename FrameCounter<Clock>::duration FrameCounter<Clock>::dt() const {
    if (timestamps_.size() < 2)
        return duration::zero();
    return timestamps_.back() - timestamps_[timestamps_.size() - 2];
}

template<typename FrameCounter>
double as_secs_dt(typename FrameCounter::duration d) {
    return static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(d).count() / 1e9);
}

template<typename T, typename Clock = std::chrono::steady_clock>
class MaxPeriod {
public:
    using time_point = typename Ticker<Clock>::time_point;
    using duration = typename Ticker<Clock>::duration;

    explicit MaxPeriod(duration period);

    void update(T v);

    T value() const;

    void debug_print() const;

private:
    std::deque<std::tuple<time_point, T>> samples_{};
    std::unordered_map<T, std::size_t> frequency_{};
    duration period_;

    void inc_frequency_(T &v);
    void dec_frequency_(T &v);
};

template<typename T, typename Clock>
MaxPeriod<T, Clock>::MaxPeriod(duration period)
    : period_(period) {}

template<typename T, typename Clock>
void MaxPeriod<T, Clock>::update(T v) {
    auto now = Clock::now();
    samples_.emplace_back(now, v);
    inc_frequency_(v);

    while (!samples_.empty() && now - std::get<0>(samples_.front()) > period_) {
        dec_frequency_(std::get<1>(samples_.front()));
        samples_.pop_front();
    }
}

template<typename T, typename Clock>
T MaxPeriod<T, Clock>::value() const {
    return std::ranges::max_element(
                   frequency_, [](const auto &a, const auto &b) { return std::get<0>(a) < std::get<0>(b); }
    )->first;
}

template<typename T, typename Clock>
void MaxPeriod<T, Clock>::debug_print() const {
    for (const auto &[k, v]: frequency_)
        MIZU_LOG_INFO("{}: {}", k, v);
}

template<typename T, typename Clock>
void MaxPeriod<T, Clock>::inc_frequency_(T &v) {
    auto it = frequency_.find(v);
    if (it == frequency_.end())
        it = frequency_.emplace_hint(it, v, 0);
    ++it->second;
}

template<typename T, typename Clock>
void MaxPeriod<T, Clock>::dec_frequency_(T &v) {
    auto it = frequency_.find(v);
    assert(it != frequency_.end());
    if (--it->second == 0)
        frequency_.erase(it);
}

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

#endif // MIZU_TIME_HPP
