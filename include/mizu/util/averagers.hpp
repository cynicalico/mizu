#ifndef MIZU_AVERAGERS_HPP
#define MIZU_AVERAGERS_HPP

#include <deque>

namespace mizu {
class CMA {
public:
    explicit CMA(std::size_t sample_count);

    void update(double v);

    double value() const;

    std::size_t samples() const;

private:
    double value_;
    std::size_t sample_count_;
};

class EMA {
public:
    double alpha;

    explicit EMA(double alpha);

    void update(double v);

    double value() const;

private:
    double value_;
};

class SMA {
public:
    std::size_t sample_count;

    explicit SMA(std::size_t sample_count);

    void update(double v);

    double value() const;

private:
    std::deque<double> samples_;
    double value_;
};
} // namespace mizu

#endif // MIZU_AVERAGERS_HPP
