#include <fmt/format.h>
#include "mizu/ds/priority_queue.hpp"

int main(int, char *[]) {
    auto pq = mizu::ds::PriorityQueue<int, std::greater>();
    pq.push(1, 10, 0);
    pq.push(2, 20, 1);
    pq.push(3, 30, 2);
    pq.update(1, 21);
    pq.update(3, 19);

    std::optional<mizu::ds::PriorityQueue<int, std::greater>::Node> node;
    while ((node = pq.pop_value()) != std::nullopt)
        fmt::println("{} {} {}", node->key, node->priority, node->value);
}
