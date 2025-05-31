#ifndef MIZU_TIME_HPP
#define MIZU_TIME_HPP

#include <chrono>
#include <deque>
#include "mizu/averagers.hpp"

namespace mizu {
template<typename Clock = std::chrono::steady_clock>
class Ticker {
public:
    using time_point = std::chrono::time_point<Clock>;
    using duration = typename Clock::duration;

    explicit Ticker(duration interval = duration::zero());

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
} // namespace mizu

#endif // MIZU_TIME_HPP
