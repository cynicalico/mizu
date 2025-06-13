#include "mizu/util/averagers.hpp"

namespace mizu {
CMA::CMA(std::size_t sample_count)
    : value_(0), sample_count_(sample_count) {}

void CMA::update(double v) {
    value_ += (v - value_) / ++sample_count_;
}

double CMA::value() const {
    return value_;
}

std::size_t CMA::samples() const {
    return sample_count_;
}

EMA::EMA(double alpha)
    : alpha(alpha), value_(0) {}

void EMA::update(double v) {
    value_ = alpha * v + (1 - alpha) * value_;
}

double EMA::value() const {
    return value_;
}

SMA::SMA(std::size_t sample_count)
    : sample_count(sample_count), value_(0) {}

void SMA::update(double v) {
    samples_.push_back(v);
    if (samples_.size() <= sample_count) {
        value_ += (v - value_) / static_cast<double>(samples_.size());
    } else {
        value_ += 1.0 / static_cast<double>(samples_.size()) * (v - samples_.front());
        samples_.pop_back();
    }
}

double SMA::value() const {
    return value_;
}
} // namespace mizu
