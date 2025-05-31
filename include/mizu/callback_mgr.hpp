#ifndef MIZU_PUBSUB_HPP
#define MIZU_PUBSUB_HPP

#include <array>
#include <functional>
#include <optional>
#include <vector>
#include "mizu/log.hpp"

namespace mizu {
class CallbackMgr {
public:
    static constexpr std::size_t MAX_BUFFER_SIZE = 1024;

    template<typename T>
    struct Buffer {
        std::size_t idx;
        bool active;
        std::array<T, MAX_BUFFER_SIZE> data;

        Buffer()
            : idx(0), active(false), data() {}

        template<typename... Args>
        void push(Args &&...args) {
            if (idx == MAX_BUFFER_SIZE) {
                // typeid(T).name is just to give an idea of what buffer it is,
                // this is not practically going to give a good name
                // TODO: provide a way to give a better name to this message
                SPDLOG_WARN("Callback buffer full {}", typeid(T).name());
                return;
            }
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

    std::size_t reg() {
        if (!recycled_ids_.empty()) {
            auto id = recycled_ids_.back();
            recycled_ids_.pop_back();
            return id;
        }
        return next_id_++;
    }

    void unreg(std::size_t id) { recycled_ids_.push_back(id); }

    template<typename T>
    void sub(std::size_t id, Callback<T> &&callback);

    template<typename T>
    void unsub(std::size_t id);

    template<typename T, typename... Args>
    void pub(Args &&...args);

    template<typename T, typename... Args>
    void pub_nowait(Args &&...args);

    template<typename T>
    void poll(std::size_t id);

private:
    std::size_t next_id_{1};
    std::vector<std::size_t> recycled_ids_{};

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
    buffers_<T>()[id].active = true;
}

template<typename T>
void CallbackMgr::unsub(std::size_t id) {
    auto &callbacks = callbacks_<T>();
    if (id < callbacks.size()) {
        callbacks[id] = nullptr;
        buffers_<T>()[id].active = false;
    }
}

template<typename T, typename... Args>
void CallbackMgr::pub(Args &&...args) {
    for (auto &buffers = buffers_<T>(); auto &b: buffers)
        if (b.active)
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
        callback(*payload);
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
