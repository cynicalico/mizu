#ifndef MIZU_DEAR_HPP
#define MIZU_DEAR_HPP

#include <SDL3/SDL_events.h>
#include <imgui.h>
#include "glm/vec2.hpp"
#include "mizu/core/callback_mgr.hpp"
#include "mizu/util/class_helpers.hpp"
#include "mizu/core/window.hpp"

namespace mizu {
class Dear {
public:
    Dear(CallbackMgr &callbacks, Window *window);
    ~Dear();

    NO_COPY(Dear)
    NO_MOVE(Dear)

    void process_sdl3_event(const SDL_Event &event);
    bool ignore_mouse_inputs() const;
    bool ignore_keyboard_inputs() const;

private:
    ImGuiContext *ctx_;
    ImGuiIO *io_;

    std::size_t callback_id_{0};
    CallbackMgr &callbacks_;

    void pre_draw_();
    void draw_();

    void register_callbacks_();
    void unregister_callbacks_();
};

namespace dear {
namespace internal {
template<typename Base, bool ForceDtor = false>
struct ScopeWrapper {
    using self_type = ScopeWrapper<Base>;

    static constexpr bool force_dtor = ForceDtor;

protected:
    const bool ok_;

public:
    explicit constexpr ScopeWrapper(bool ok) noexcept
        : ok_{ok} {}

    ~ScopeWrapper() noexcept {
        if constexpr (!force_dtor) {
            if (!ok_)
                return;
        }
        Base::dtor();
    }

    template<typename F>
    constexpr bool operator&&(F f) const noexcept {
        if (ok_)
            f();
        return ok_;
    }

    constexpr operator bool() const noexcept { return ok_; }

protected:
    NO_COPY(ScopeWrapper)
    NO_MOVE(ScopeWrapper)
};
} // namespace internal

struct begin : internal::ScopeWrapper<begin, true> {
    explicit begin(const char *title, bool *open = nullptr, ImGuiWindowFlags flags = ImGuiWindowFlags_None) noexcept
        : ScopeWrapper(ImGui::Begin(title, open, flags)) {}
    static void dtor() noexcept { ImGui::End(); }
};

struct child : internal::ScopeWrapper<child, true> {
    explicit child(
            const char *title,
            const ImVec2 &size = glm::vec2(0, 0),
            ImGuiChildFlags child_flags = ImGuiChildFlags_None,
            ImGuiWindowFlags flags = ImGuiWindowFlags_None
    )
        : ScopeWrapper(ImGui::BeginChild(title, size, child_flags, flags)) {}
    static void dtor() noexcept { ImGui::EndChild(); }
};

struct group : internal::ScopeWrapper<group, true> {
    group() noexcept
        : ScopeWrapper(true) {
        ImGui::BeginGroup();
    }
    static void dtor() noexcept { ImGui::EndGroup(); }
};

struct combo : internal::ScopeWrapper<combo> {
    combo(const char *label, const char *preview, ImGuiComboFlags flags = ImGuiComboFlags_None) noexcept
        : ScopeWrapper(ImGui::BeginCombo(label, preview, flags)) {}
    static void dtor() noexcept { ImGui::EndCombo(); }
};
} // namespace dear
} // namespace mizu

#endif // MIZU_DEAR_HPP
