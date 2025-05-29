#include "mizu/dear.hpp"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include "mizu/log.hpp"
#include "mizu/payloads.hpp"

namespace mizu {
Dear::Dear(CallbackMgr &callbacks, Window *window)
    : callbacks_(callbacks) {
    register_callbacks_();

    IMGUI_CHECKVERSION();
    ctx_ = ImGui::CreateContext();
    SPDLOG_DEBUG("Created Dear ImGui context");
    io_ = &ImGui::GetIO();
    io_->IniFilename = nullptr;

    ImGui_ImplSDL3_InitForOpenGL(window->underlying(), window->gl_context());
    const char *glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

    SPDLOG_DEBUG("Initialized Dear ImGui");
}

Dear::~Dear() {
    unregister_callbacks_();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    SPDLOG_DEBUG("Shutdown Dear ImGui");

    if (ctx_) {
        ImGui::DestroyContext(ctx_);
        SPDLOG_DEBUG("Destroyed Dear ImGui context");
    }
}

void Dear::process_sdl3_event(const SDL_Event &event) {
    ImGui_ImplSDL3_ProcessEvent(&event);
}

bool Dear::ignore_mouse_inputs() const {
    return io_->WantCaptureMouse;
}

bool Dear::ignore_keyboard_inputs() const {
    return io_->WantCaptureKeyboard;
}

void Dear::pre_draw_() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void Dear::draw_() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Dear::register_callbacks_() {
    callback_id_ = callbacks_.reg();
    callbacks_.sub<PPreDrawOverlay>(callback_id_, [&](const auto &) { pre_draw_(); });
    callbacks_.sub<PDrawOverlay>(callback_id_, [&](const auto &) { draw_(); });
}

void Dear::unregister_callbacks_() {
    callbacks_.unsub<PDrawOverlay>(callback_id_);
    callbacks_.unsub<PPreDrawOverlay>(callback_id_);
    callbacks_.unreg(callback_id_);
    callback_id_ = 0;
}
} // namespace mizu
