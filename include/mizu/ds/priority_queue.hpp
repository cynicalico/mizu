#ifndef DS_PRIORITY_QUEUE_HPP
#define DS_PRIORITY_QUEUE_HPP

#include <optional>
#include <vector>

#define PARENT(x) ((x - 1) / 2)
#define LEFT(x) (x * 2 + 1)
#define RIGHT(x) (x * 2 + 2)

namespace mizu::ds {
template<typename T, template<typename V> class Compare = std::less>
class PriorityQueue {
public:
    const std::size_t HEAP_POS_NONE = std::numeric_limits<std::size_t>::max();

    struct Node {
        std::size_t key;
        std::size_t priority;
        T value;

        Node(std::size_t key, std::size_t priority, T value) : key(key), priority(priority), value(value) {}
    };

    PriorityQueue() = default;

    std::size_t size() const { return heap_.size(); }

    bool empty() const { return size() == 0; }

    const Node &top() const { return heap_.front(); }

    void pop() {
        if (empty()) return;

        key_to_heap_pos_[heap_.front().key] = HEAP_POS_NONE;
        if (size() > 1) {
            *heap_.begin() = std::move(*(heap_.end() - 1));
            key_to_heap_pos_[heap_.front().key] = 0;
        }
        heap_.pop_back();
        sift_down(0);
    }

    std::optional<Node> pop_value() {
        if (empty()) return std::nullopt;

        Node ret = std::move(*heap_.begin());
        key_to_heap_pos_[ret.key] = HEAP_POS_NONE;
        if (size() > 1) {
            *heap_.begin() = std::move(*(heap_.end() - 1));
            key_to_heap_pos_[heap_.front().key] = 0;
        }
        heap_.pop_back();
        sift_down(0);

        return ret;
    }

    bool push_or_update(std::size_t key, std::size_t priority, bool only_if_higher = false) {
        if (key < key_to_heap_pos_.size() && key_to_heap_pos_[key] != HEAP_POS_NONE)
            return update(key, priority, only_if_higher);

        return push(key, priority);
    }

    std::optional<std::size_t> get_priority(std::size_t key) {
        if (key < key_to_heap_pos_.size()) {
            if (size_t pos = key_to_heap_pos_[key]; pos != HEAP_POS_NONE) return heap_[pos].priority;
        }
        return std::nullopt;
    }

    bool push(std::size_t key, std::size_t priority, T value) {
        check_resize_ids(key);

        if (key_to_heap_pos_[key] != HEAP_POS_NONE) return false;

        std::size_t pos = heap_.size();
        key_to_heap_pos_[key] = pos;
        heap_.emplace_back(key, priority, value);
        sift_up(pos);

        return true;
    }

    bool update(std::size_t key, std::size_t new_priority, bool only_if_higher = false) {
        if (key >= key_to_heap_pos_.size()) return false;

        std::size_t heap_pos = key_to_heap_pos_[key];
        if (heap_pos == HEAP_POS_NONE) return false;

        std::size_t &priority = heap_[heap_pos].priority;
        if (!comp_f_(new_priority, priority)) {
            priority = new_priority;
            sift_up(heap_pos);
            return true;
        }

        if (!only_if_higher && comp_f_(new_priority, priority)) {
            priority = new_priority;
            sift_down(heap_pos);
            return true;
        }

        return false;
    }

private:
    Compare<std::size_t> comp_f_{};
    std::vector<size_t> key_to_heap_pos_{};
    std::vector<Node> heap_{};

    void check_resize_ids(std::size_t needed_capacity) {
        if (key_to_heap_pos_.size() < needed_capacity + 1) key_to_heap_pos_.resize(needed_capacity + 1, HEAP_POS_NONE);
    }

    void sift_down(std::size_t heap_pos) {
        if (size() <= 1) return;

        std::size_t child = greater_priority_child_(heap_pos);

        while (child < size() && !comp_f_(heap_[child].priority, heap_[heap_pos].priority)) {
            std::swap(heap_[child], heap_[heap_pos]);
            std::swap(key_to_heap_pos_[heap_[child].key], key_to_heap_pos_[heap_[heap_pos].key]);

            heap_pos = child;
            child = greater_priority_child_(heap_pos);
        }
    }

    void sift_up(std::size_t heap_pos) {
        if (size() <= 1) return;

        std::size_t parent = PARENT(heap_pos);

        while (heap_pos > 0 && !comp_f_(heap_[heap_pos].priority, heap_[parent].priority)) {
            std::swap(heap_[heap_pos], heap_[parent]);
            std::swap(key_to_heap_pos_[heap_[heap_pos].key], key_to_heap_pos_[heap_[parent].key]);

            heap_pos = parent;
            parent = PARENT(heap_pos);
        }
    }

    std::size_t greater_priority_child_(std::size_t heap_pos) {
        auto left = LEFT(heap_pos);
        auto right = RIGHT(heap_pos);

        if (left >= size()) return HEAP_POS_NONE;

        if (right < size() && !comp_f_(heap_[right].priority, heap_[left].priority)) return right;

        return left;
    }
};
} // namespace mizu::ds

#undef PARENT
#undef LEFT
#undef RIGHT

#endif // DS_PRIORITY_QUEUE_HPP
