#ifndef MIZU_PAYLOADS_HPP
#define MIZU_PAYLOADS_HPP

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_scancode.h>

namespace mizu {
/*****************
 * ENGINE STAGES *
 *****************/

struct PPreUpdate {
    double dt;
};

struct PUpdate {
    double dt;
};

struct PPostUpdate {
    double dt;
};

struct PPreDraw {};

struct PPreDrawOverlay {};

struct PDraw {};

struct PDrawOverlay {};

struct PPostDraw {};

struct PPresent {};

/******************************************
 * SDL EVENTS                             *
 * https://wiki.libsdl.org/SDL3/SDL_Event *
 ******************************************/

// https://wiki.libsdl.org/SDL3/SDL_KeyboardEvent
struct PEventKeyDown {
    std::uint64_t timestamp;
    SDL_Scancode scancode;
    SDL_Keycode key;
    SDL_Keymod mod;
    bool repeat;
};

// https://wiki.libsdl.org/SDL3/SDL_KeyboardEvent
struct PEventKeyUp {
    std::uint64_t timestamp;
    SDL_Scancode scancode;
    SDL_Keycode key;
    SDL_Keymod mod;
};

// https://wiki.libsdl.org/SDL3/SDL_MouseMotionEvent
struct PEventMouseMotion {
    std::uint64_t timestamp;
    float x;
    float y;
    float dx;
    float dy;
};

// https://wiki.libsdl.org/SDL3/SDL_MouseButtonEvent
struct PEventMouseButtonDown {
    std::uint64_t timestamp;
    std::uint8_t button;
    std::uint8_t clicks;
    float x;
    float y;
};

// https://wiki.libsdl.org/SDL3/SDL_MouseButtonEvent
struct PEventMouseButtonUp {
    std::uint64_t timestamp;
    std::uint8_t button;
    float x;
    float y;
};

// https://wiki.libsdl.org/SDL3/SDL_MouseWheelEvent
struct PEventMouseWheel {
    std::uint64_t timestamp;
    bool natural;
    float x;
    float y;
    float mouse_x;
    float mouse_y;
};

// https://wiki.libsdl.org/SDL3/SDL_WindowEvent
struct PEventMouseEnter {
    std::uint64_t timestamp;
};

// https://wiki.libsdl.org/SDL3/SDL_WindowEvent
struct PEventMouseLeave {
    std::uint64_t timestamp;
};

// https://wiki.libsdl.org/SDL3/SDL_WindowEvent
struct PEventWindowResized {
    std::uint64_t timestamp;
    int width;
    int height;
};

// https://wiki.libsdl.org/SDL3/SDL_WindowEvent
struct PEventWindowPixelSizeChanged {
    std::uint64_t timestamp;
    int width;
    int height;
};

// https://wiki.libsdl.org/SDL3/SDL_WindowEvent
struct PEventWindowFocusGained {
    std::uint64_t timestamp;
};

// https://wiki.libsdl.org/SDL3/SDL_WindowEvent
struct PEventWindowFocusLost {
    std::uint64_t timestamp;
};

// https://wiki.libsdl.org/SDL3/SDL_QuitEvent
struct PEventQuit {
    std::uint64_t timestamp;
};

} // namespace mizu

#endif // MIZU_PAYLOADS_HPP
