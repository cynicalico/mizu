#include <fmt/format.h>
#include <functional>
#include "mizu/ds/priority_queue.hpp"
#include "mizu/id_mgr.hpp"
#include "mizu/module.hpp"

class Foo final : public mizu::Module {
public:
    std::string name() const override { return "Foo"; }
};

class Bar final : public mizu::Module {
public:
    std::string name() const override { return "Bar"; }
};

class Baz final : public mizu::Module {
public:
    std::string name() const override { return "Baz"; }
};

int main(int, char *[]) {
    // Foo : { Bar, Baz }
    // Baz : { Bar }
    // Bar : {}

    auto ids = mizu::IdMgr();
    auto pq = mizu::ds::PriorityQueue<std::function<void()>, std::greater>();

    auto foo = Foo();
    auto bar = Bar();
    auto baz = Baz();

    pq.push(ids.get(bar), 0, [&] { fmt::println("{}", bar.name()); });
    pq.push(ids.get(baz), 1, [&] { fmt::println("{}", baz.name()); });
    pq.push(ids.get(foo), 2, [&] { fmt::println("{}", foo.name()); });

    std::optional<decltype(pq)::Node> node;
    while ((node = pq.pop_value()) != std::nullopt)
        node->value();

    pq.push(ids.get(foo), 0, [&] { fmt::println("{}", foo.name()); });

    pq.update(ids.get(foo), pq.get_priority(ids.get(foo)).value() + 1);
    pq.push(ids.get(baz), 0, [&] { fmt::println("{}", baz.name()); });

    pq.update(ids.get(foo), pq.get_priority(ids.get(foo)).value() + 1);
    pq.update(ids.get(baz), pq.get_priority(ids.get(baz)).value() + 1);
    pq.push(ids.get(bar), 0, [&] { fmt::println("{}", bar.name()); });

    // std::optional<decltype(pq)::Node> node;
    while ((node = pq.pop_value()) != std::nullopt)
        node->value();

    // auto pq = mizu::ds::PriorityQueue<int, std::greater>();
    // pq.push(1, 10, 0);
    // pq.push(2, 20, 1);
    // pq.push(3, 30, 2);
    // pq.update(1, 21);
    // pq.update(3, 19);
    //
    // std::optional<mizu::ds::PriorityQueue<int, std::greater>::Node> node;
    // while ((node = pq.pop_value()) != std::nullopt)
    //     fmt::println("{} {} {}", node->key, node->priority, node->value);
}
