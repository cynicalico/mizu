#ifndef MIZU_GUI_LAYOUT_HPP
#define MIZU_GUI_LAYOUT_HPP

#include <optional>
#include "mizu/gui/node.hpp"

namespace mizu::gui {
struct VStackParams {
    std::optional<Border> border{std::nullopt};
    Padding outer_pad{0.0f};
    float inner_pad{0.0f};
};

class VStack final : public Node<VStackParams> {
public:
    std::optional<Border> border;
    Padding outer_pad;
    float inner_pad;

    explicit VStack(VStackParams params);

    void resize(const glm::vec2 &max_size_hint) override;

    void calc_bbox(glm::vec2 pos) override;

    void update(InputMgr &input) override;

    void draw(G2d &g2d) const override;

private:
    float border_size_() const;
};

struct HStackParams {
    std::optional<Border> border{std::nullopt};
    Padding outer_pad{0.0f};
    float inner_pad{0.0f};
};

class HStack final : public Node<HStackParams> {
public:
    std::optional<Border> border;
    Padding outer_pad;
    float inner_pad;

    explicit HStack(HStackParams params);

    void resize(const glm::vec2 &max_size_hint) override;

    void calc_bbox(glm::vec2 pos) override;

    void update(InputMgr &input) override;

    void draw(G2d &g2d) const override;

private:
    float border_size_() const;
};

class VSpacer final : public Node<void> {
public:
    VSpacer();

    void resize(const glm::vec2 &max_size_hint) override;

    void calc_bbox(glm::vec2 pos) override;

    void update(InputMgr &input) override;

    void draw(G2d &g2d) const override;
};

class HSpacer final : public Node<void> {
public:
    HSpacer();

    void resize(const glm::vec2 &max_size_hint) override;

    void calc_bbox(glm::vec2 pos) override;

    void update(InputMgr &input) override;

    void draw(G2d &g2d) const override;
};
} // namespace mizu::gui

#endif // MIZU_GUI_LAYOUT_HPP
