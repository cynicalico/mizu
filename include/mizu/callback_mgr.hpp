#ifndef MIZU_PUBSUB_HPP
#define MIZU_PUBSUB_HPP

#include <array>
#include <functional>
#include <optional>
#include <vector>

namespace mizu {
class CallbackMgr {
public:
    static constexpr std::size_t MAX_BUFFER_SIZE = 1024;

    template<typename T>
    struct Buffer {
        std::size_t idx;
        std::array<T, MAX_BUFFER_SIZE> data;

        Buffer() : idx(0), data() {}

        template<typename... Args>
        void push(Args &&...args) {
            if (idx == MAX_BUFFER_SIZE)
                return;
            data[idx++] = T(std::forward<Args>(args)...);
        }

        std::optional<T> pop() {
            if (idx == 0)
                return std::nullopt;
            return std::make_optional(data[--idx]);
        }
    };

    template<typename T>
    using Callback = std::function<void(const T &)>;

    std::size_t reg() { return next_id_++; }

    template<typename T>
    void sub(std::size_t id, Callback<T> &&callback);

    template<typename T, typename... Args>
    void pub(Args &&...args);

    template<typename T, typename... Args>
    void pub_nowait(Args &&...args);

    template<typename T>
    void poll(std::size_t id);

private:
    std::size_t next_id_{0};

    template<typename T>
    std::vector<Callback<T>> &callbacks_();

    template<typename T>
    std::vector<Buffer<T>> &buffers_();
};

template<typename T>
void CallbackMgr::sub(std::size_t id, Callback<T> &&callback) {
    auto &callbacks = callbacks_<T>();
    if (id >= callbacks.size()) {
        callbacks.resize(id + 1, nullptr);
        buffers_<T>().resize(id + 1);
    }
    callbacks[id] = std::forward<Callback<T>>(callback);
}

template<typename T, typename... Args>
void CallbackMgr::pub(Args &&...args) {
    for (auto &buffers = buffers_<T>(); auto &b: buffers)
        b.push(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void CallbackMgr::pub_nowait(Args &&...args) {
    auto payload = T{std::forward<Args>(args)...};
    for (auto &callbacks = callbacks_<T>(); const auto &c: callbacks)
        if (c)
            c(payload);
}

template<typename T>
void CallbackMgr::poll(std::size_t id) {
    auto &callbacks = callbacks_<T>();
    if (id >= callbacks.size())
        return;
    auto &callback = callbacks[id];

    auto &buffer = buffers_<T>()[id];
    std::optional<T> payload;
    while ((payload = buffer.pop()) != std::nullopt)
        callback(payload);
}

template<typename T>
std::vector<CallbackMgr::Callback<T>> &CallbackMgr::callbacks_() {
    static std::vector<Callback<T>> callbacks{};
    return callbacks;
}

template<typename T>
std::vector<CallbackMgr::Buffer<T>> &CallbackMgr::buffers_() {
    static std::vector<Buffer<T>> buffers{};
    return buffers;
}

} // namespace mizu

#endif // MIZU_PUBSUB_HPP
