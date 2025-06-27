#ifndef MIZU_INPUT_TYPES_HPP
#define MIZU_INPUT_TYPES_HPP

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include "fmt/core.h"
#include "mizu/util/enum_class_helpers.hpp"

namespace mizu {
enum class Key : int {
    Unknown = SDLK_UNKNOWN,
    Return = SDLK_RETURN,
    Escape = SDLK_ESCAPE,
    Backspace = SDLK_BACKSPACE,
    Tab = SDLK_TAB,
    Space = SDLK_SPACE,
    Exclaim = SDLK_EXCLAIM,
    DblApostrophe = SDLK_DBLAPOSTROPHE,
    Hash = SDLK_HASH,
    Dollar = SDLK_DOLLAR,
    Percent = SDLK_PERCENT,
    Ampersand = SDLK_AMPERSAND,
    Apostrophe = SDLK_APOSTROPHE,
    LeftParen = SDLK_LEFTPAREN,
    RightParen = SDLK_RIGHTPAREN,
    Asterisk = SDLK_ASTERISK,
    Plus = SDLK_PLUS,
    Comma = SDLK_COMMA,
    Minus = SDLK_MINUS,
    Period = SDLK_PERIOD,
    Slash = SDLK_SLASH,
    N0 = SDLK_0,
    N1 = SDLK_1,
    N2 = SDLK_2,
    N3 = SDLK_3,
    N4 = SDLK_4,
    N5 = SDLK_5,
    N6 = SDLK_6,
    N7 = SDLK_7,
    N8 = SDLK_8,
    N9 = SDLK_9,
    Colon = SDLK_COLON,
    SemiColon = SDLK_SEMICOLON,
    Less = SDLK_LESS,
    Equals = SDLK_EQUALS,
    Greater = SDLK_GREATER,
    Question = SDLK_QUESTION,
    At = SDLK_AT,
    LeftBracket = SDLK_LEFTBRACKET,
    Backslash = SDLK_BACKSLASH,
    RightBracket = SDLK_RIGHTBRACKET,
    Caret = SDLK_CARET,
    Underscore = SDLK_UNDERSCORE,
    Grave = SDLK_GRAVE,
    A = SDLK_A,
    B = SDLK_B,
    C = SDLK_C,
    D = SDLK_D,
    E = SDLK_E,
    F = SDLK_F,
    G = SDLK_G,
    H = SDLK_H,
    I = SDLK_I,
    J = SDLK_J,
    K = SDLK_K,
    L = SDLK_L,
    M = SDLK_M,
    N = SDLK_N,
    O = SDLK_O,
    P = SDLK_P,
    Q = SDLK_Q,
    R = SDLK_R,
    S = SDLK_S,
    T = SDLK_T,
    U = SDLK_U,
    V = SDLK_V,
    W = SDLK_W,
    X = SDLK_X,
    Y = SDLK_Y,
    Z = SDLK_Z,
    LeftBrace = SDLK_LEFTBRACE,
    Pipe = SDLK_PIPE,
    RightBrace = SDLK_RIGHTBRACE,
    Tilde = SDLK_TILDE,
    Delete = SDLK_DELETE,
    PlusMinus = SDLK_PLUSMINUS,
    CapsLock = SDLK_CAPSLOCK,
    F1 = SDLK_F1,
    F2 = SDLK_F2,
    F3 = SDLK_F3,
    F4 = SDLK_F4,
    F5 = SDLK_F5,
    F6 = SDLK_F6,
    F7 = SDLK_F7,
    F8 = SDLK_F8,
    F9 = SDLK_F9,
    F10 = SDLK_F10,
    F11 = SDLK_F11,
    F12 = SDLK_F12,
    PrintScreen = SDLK_PRINTSCREEN,
    ScrollLock = SDLK_SCROLLLOCK,
    Pause = SDLK_PAUSE,
    Insert = SDLK_INSERT,
    Home = SDLK_HOME,
    PageUp = SDLK_PAGEUP,
    End = SDLK_END,
    PageDown = SDLK_PAGEDOWN,
    Right = SDLK_RIGHT,
    Left = SDLK_LEFT,
    Down = SDLK_DOWN,
    Up = SDLK_UP,
    NumLockClear = SDLK_NUMLOCKCLEAR,
    KpDivide = SDLK_KP_DIVIDE,
    KpMultiply = SDLK_KP_MULTIPLY,
    KpMinus = SDLK_KP_MINUS,
    KpPlus = SDLK_KP_PLUS,
    KpEnter = SDLK_KP_ENTER,
    Kp1 = SDLK_KP_1,
    Kp2 = SDLK_KP_2,
    Kp3 = SDLK_KP_3,
    Kp4 = SDLK_KP_4,
    Kp5 = SDLK_KP_5,
    Kp6 = SDLK_KP_6,
    Kp7 = SDLK_KP_7,
    Kp8 = SDLK_KP_8,
    Kp9 = SDLK_KP_9,
    Kp0 = SDLK_KP_0,
    KpPeriod = SDLK_KP_PERIOD,
    Application = SDLK_APPLICATION,
    Power = SDLK_POWER,
    KpEquals = SDLK_KP_EQUALS,
    F13 = SDLK_F13,
    F14 = SDLK_F14,
    F15 = SDLK_F15,
    F16 = SDLK_F16,
    F17 = SDLK_F17,
    F18 = SDLK_F18,
    F19 = SDLK_F19,
    F20 = SDLK_F20,
    F21 = SDLK_F21,
    F22 = SDLK_F22,
    F23 = SDLK_F23,
    F24 = SDLK_F24,
    Execute = SDLK_EXECUTE,
    Help = SDLK_HELP,
    Menu = SDLK_MENU,
    Select = SDLK_SELECT,
    Stop = SDLK_STOP,
    Again = SDLK_AGAIN,
    Undo = SDLK_UNDO,
    Cut = SDLK_CUT,
    Copy = SDLK_COPY,
    Paste = SDLK_PASTE,
    Find = SDLK_FIND,
    Mute = SDLK_MUTE,
    VolumeUp = SDLK_VOLUMEUP,
    VolumeDown = SDLK_VOLUMEDOWN,
    KpComma = SDLK_KP_COMMA,
    KpEqualsAs400 = SDLK_KP_EQUALSAS400,
    AltErase = SDLK_ALTERASE,
    SysReq = SDLK_SYSREQ,
    Cancel = SDLK_CANCEL,
    Clear = SDLK_CLEAR,
    Prior = SDLK_PRIOR,
    Return2 = SDLK_RETURN2,
    Separator = SDLK_SEPARATOR,
    Out = SDLK_OUT,
    Oper = SDLK_OPER,
    ClearAgain = SDLK_CLEARAGAIN,
    CrSel = SDLK_CRSEL,
    ExSel = SDLK_EXSEL,
    Kp00 = SDLK_KP_00,
    Kp000 = SDLK_KP_000,
    ThousandsSeparator = SDLK_THOUSANDSSEPARATOR,
    DecimalSeparator = SDLK_DECIMALSEPARATOR,
    CurrencyUnit = SDLK_CURRENCYUNIT,
    CurrencySubUnit = SDLK_CURRENCYSUBUNIT,
    KpLeftParen = SDLK_KP_LEFTPAREN,
    KpRightParen = SDLK_KP_RIGHTPAREN,
    KpLeftBrace = SDLK_KP_LEFTBRACE,
    KpRightBrace = SDLK_KP_RIGHTBRACE,
    KpTab = SDLK_KP_TAB,
    KpBackspace = SDLK_KP_BACKSPACE,
    KpA = SDLK_KP_A,
    KpB = SDLK_KP_B,
    KpC = SDLK_KP_C,
    KpD = SDLK_KP_D,
    KpE = SDLK_KP_E,
    KpF = SDLK_KP_F,
    KpXor = SDLK_KP_XOR,
    KpPower = SDLK_KP_POWER,
    KpPercent = SDLK_KP_PERCENT,
    KpLess = SDLK_KP_LESS,
    KpGreater = SDLK_KP_GREATER,
    KpAmpersand = SDLK_KP_AMPERSAND,
    KpDblAmpersand = SDLK_KP_DBLAMPERSAND,
    KpVerticalBar = SDLK_KP_VERTICALBAR,
    KpDblVerticalBar = SDLK_KP_DBLVERTICALBAR,
    KpColon = SDLK_KP_COLON,
    KpHash = SDLK_KP_HASH,
    KpSpace = SDLK_KP_SPACE,
    KpAt = SDLK_KP_AT,
    KpExclam = SDLK_KP_EXCLAM,
    KpMemStore = SDLK_KP_MEMSTORE,
    KpMemRecall = SDLK_KP_MEMRECALL,
    KpMemClear = SDLK_KP_MEMCLEAR,
    KpMemAdd = SDLK_KP_MEMADD,
    KpMemSubtract = SDLK_KP_MEMSUBTRACT,
    KpMemMultiply = SDLK_KP_MEMMULTIPLY,
    KpMemDivide = SDLK_KP_MEMDIVIDE,
    KpPlusMinus = SDLK_KP_PLUSMINUS,
    KpClear = SDLK_KP_CLEAR,
    KpClearEntry = SDLK_KP_CLEARENTRY,
    KpBinary = SDLK_KP_BINARY,
    KpOctal = SDLK_KP_OCTAL,
    KpDecimal = SDLK_KP_DECIMAL,
    KpHexadecimal = SDLK_KP_HEXADECIMAL,
    LCtrl = SDLK_LCTRL,
    LShift = SDLK_LSHIFT,
    LAlt = SDLK_LALT,
    LGui = SDLK_LGUI,
    RCtrl = SDLK_RCTRL,
    RShift = SDLK_RSHIFT,
    RAlt = SDLK_RALT,
    RGui = SDLK_RGUI,
    Mode = SDLK_MODE,
    Sleep = SDLK_SLEEP,
    Wake = SDLK_WAKE,
    ChannelIncrement = SDLK_CHANNEL_INCREMENT,
    ChannelDecrement = SDLK_CHANNEL_DECREMENT,
    MediaPlay = SDLK_MEDIA_PLAY,
    MediaPause = SDLK_MEDIA_PAUSE,
    MediaRecord = SDLK_MEDIA_RECORD,
    MediaFastForward = SDLK_MEDIA_FAST_FORWARD,
    MediaRewind = SDLK_MEDIA_REWIND,
    MediaNextTrack = SDLK_MEDIA_NEXT_TRACK,
    MediaPreviousTrack = SDLK_MEDIA_PREVIOUS_TRACK,
    MediaStop = SDLK_MEDIA_STOP,
    MediaEject = SDLK_MEDIA_EJECT,
    MediaPlayPause = SDLK_MEDIA_PLAY_PAUSE,
    MediaSelect = SDLK_MEDIA_SELECT,
    AcNew = SDLK_AC_NEW,
    AcOpen = SDLK_AC_OPEN,
    AcClose = SDLK_AC_CLOSE,
    AcExit = SDLK_AC_EXIT,
    AcSave = SDLK_AC_SAVE,
    AcPrint = SDLK_AC_PRINT,
    AcProperties = SDLK_AC_PROPERTIES,
    AcSearch = SDLK_AC_SEARCH,
    AcHome = SDLK_AC_HOME,
    AcBack = SDLK_AC_BACK,
    AcForward = SDLK_AC_FORWARD,
    AcStop = SDLK_AC_STOP,
    AcRefresh = SDLK_AC_REFRESH,
    AcBookmarks = SDLK_AC_BOOKMARKS,
    SoftLeft = SDLK_SOFTLEFT,
    SoftRight = SDLK_SOFTRIGHT,
    Call = SDLK_CALL,
    EndCall = SDLK_ENDCALL,
    LeftTab = SDLK_LEFT_TAB,
    Level5Shift = SDLK_LEVEL5_SHIFT,
    MultiKeyCompose = SDLK_MULTI_KEY_COMPOSE,
    LMeta = SDLK_LMETA,
    RMeta = SDLK_RMETA,
    LHyper = SDLK_LHYPER,
    RHyper = SDLK_RHYPER,
};

enum class MouseButton : int {
    Left = SDL_BUTTON_LEFT,
    Middle = SDL_BUTTON_MIDDLE,
    Right = SDL_BUTTON_RIGHT,
    X1 = SDL_BUTTON_X1,
    X2 = SDL_BUTTON_X2
};

enum class Mod : int {
    None = SDL_KMOD_NONE,
    LShift = SDL_KMOD_LSHIFT,
    RShift = SDL_KMOD_RSHIFT,
    Level5 = SDL_KMOD_LEVEL5,
    LCtrl = SDL_KMOD_LCTRL,
    RCtrl = SDL_KMOD_RCTRL,
    LAlt = SDL_KMOD_LALT,
    RAlt = SDL_KMOD_RALT,
    LGui = SDL_KMOD_LGUI,
    RGui = SDL_KMOD_RGUI,
    Num = SDL_KMOD_NUM,
    Caps = SDL_KMOD_CAPS,
    Scroll = SDL_KMOD_SCROLL,
    Ctrl = SDL_KMOD_CTRL,
    Shift = SDL_KMOD_SHIFT,
    Alt = SDL_KMOD_ALT,
    Gui = SDL_KMOD_GUI,
};
} // namespace mizu

ENUM_CLASS_ENABLE_BITOPS(mizu::Mod);

template<>
struct fmt::formatter<mizu::Key> : formatter<string_view> {
    auto format(mizu::Key k, format_context &ctx) const -> format_context::iterator {
        string_view repr;
        switch (k) {
        case mizu::Key::Unknown: repr = "Unknown"; break;
        case mizu::Key::Return: repr = "Return"; break;
        case mizu::Key::Escape: repr = "Escape"; break;
        case mizu::Key::Backspace: repr = "Backspace"; break;
        case mizu::Key::Tab: repr = "Tab"; break;
        case mizu::Key::Space: repr = "Space"; break;
        case mizu::Key::Exclaim: repr = "Exclaim"; break;
        case mizu::Key::DblApostrophe: repr = "DblApostrophe"; break;
        case mizu::Key::Hash: repr = "Hash"; break;
        case mizu::Key::Dollar: repr = "Dollar"; break;
        case mizu::Key::Percent: repr = "Percent"; break;
        case mizu::Key::Ampersand: repr = "Ampersand"; break;
        case mizu::Key::Apostrophe: repr = "Apostrophe"; break;
        case mizu::Key::LeftParen: repr = "LeftParen"; break;
        case mizu::Key::RightParen: repr = "RightParen"; break;
        case mizu::Key::Asterisk: repr = "Asterisk"; break;
        case mizu::Key::Plus: repr = "Plus"; break;
        case mizu::Key::Comma: repr = "Comma"; break;
        case mizu::Key::Minus: repr = "Minus"; break;
        case mizu::Key::Period: repr = "Period"; break;
        case mizu::Key::Slash: repr = "Slash"; break;
        case mizu::Key::N0: repr = "0"; break;
        case mizu::Key::N1: repr = "1"; break;
        case mizu::Key::N2: repr = "2"; break;
        case mizu::Key::N3: repr = "3"; break;
        case mizu::Key::N4: repr = "4"; break;
        case mizu::Key::N5: repr = "5"; break;
        case mizu::Key::N6: repr = "6"; break;
        case mizu::Key::N7: repr = "7"; break;
        case mizu::Key::N8: repr = "8"; break;
        case mizu::Key::N9: repr = "9"; break;
        case mizu::Key::Colon: repr = "Colon"; break;
        case mizu::Key::SemiColon: repr = "SemiColon"; break;
        case mizu::Key::Less: repr = "Less"; break;
        case mizu::Key::Equals: repr = "Equals"; break;
        case mizu::Key::Greater: repr = "Greater"; break;
        case mizu::Key::Question: repr = "Question"; break;
        case mizu::Key::At: repr = "At"; break;
        case mizu::Key::LeftBracket: repr = "LeftBracket"; break;
        case mizu::Key::Backslash: repr = "Backslash"; break;
        case mizu::Key::RightBracket: repr = "RightBracket"; break;
        case mizu::Key::Caret: repr = "Caret"; break;
        case mizu::Key::Underscore: repr = "Underscore"; break;
        case mizu::Key::Grave: repr = "Grave"; break;
        case mizu::Key::A: repr = "A"; break;
        case mizu::Key::B: repr = "B"; break;
        case mizu::Key::C: repr = "C"; break;
        case mizu::Key::D: repr = "D"; break;
        case mizu::Key::E: repr = "E"; break;
        case mizu::Key::F: repr = "F"; break;
        case mizu::Key::G: repr = "G"; break;
        case mizu::Key::H: repr = "H"; break;
        case mizu::Key::I: repr = "I"; break;
        case mizu::Key::J: repr = "J"; break;
        case mizu::Key::K: repr = "K"; break;
        case mizu::Key::L: repr = "L"; break;
        case mizu::Key::M: repr = "M"; break;
        case mizu::Key::N: repr = "N"; break;
        case mizu::Key::O: repr = "O"; break;
        case mizu::Key::P: repr = "P"; break;
        case mizu::Key::Q: repr = "Q"; break;
        case mizu::Key::R: repr = "R"; break;
        case mizu::Key::S: repr = "S"; break;
        case mizu::Key::T: repr = "T"; break;
        case mizu::Key::U: repr = "U"; break;
        case mizu::Key::V: repr = "V"; break;
        case mizu::Key::W: repr = "W"; break;
        case mizu::Key::X: repr = "X"; break;
        case mizu::Key::Y: repr = "Y"; break;
        case mizu::Key::Z: repr = "Z"; break;
        case mizu::Key::LeftBrace: repr = "LeftBrace"; break;
        case mizu::Key::Pipe: repr = "Pipe"; break;
        case mizu::Key::RightBrace: repr = "RightBrace"; break;
        case mizu::Key::Tilde: repr = "Tilde"; break;
        case mizu::Key::Delete: repr = "Delete"; break;
        case mizu::Key::PlusMinus: repr = "PlusMinus"; break;
        case mizu::Key::CapsLock: repr = "CapsLock"; break;
        case mizu::Key::F1: repr = "F1"; break;
        case mizu::Key::F2: repr = "F2"; break;
        case mizu::Key::F3: repr = "F3"; break;
        case mizu::Key::F4: repr = "F4"; break;
        case mizu::Key::F5: repr = "F5"; break;
        case mizu::Key::F6: repr = "F6"; break;
        case mizu::Key::F7: repr = "F7"; break;
        case mizu::Key::F8: repr = "F8"; break;
        case mizu::Key::F9: repr = "F9"; break;
        case mizu::Key::F10: repr = "F10"; break;
        case mizu::Key::F11: repr = "F11"; break;
        case mizu::Key::F12: repr = "F12"; break;
        case mizu::Key::PrintScreen: repr = "PrintScreen"; break;
        case mizu::Key::ScrollLock: repr = "ScrollLock"; break;
        case mizu::Key::Pause: repr = "Pause"; break;
        case mizu::Key::Insert: repr = "Insert"; break;
        case mizu::Key::Home: repr = "Home"; break;
        case mizu::Key::PageUp: repr = "PageUp"; break;
        case mizu::Key::End: repr = "End"; break;
        case mizu::Key::PageDown: repr = "PageDown"; break;
        case mizu::Key::Right: repr = "Right"; break;
        case mizu::Key::Left: repr = "Left"; break;
        case mizu::Key::Down: repr = "Down"; break;
        case mizu::Key::Up: repr = "Up"; break;
        case mizu::Key::NumLockClear: repr = "NumLockClear"; break;
        case mizu::Key::KpDivide: repr = "KpDivide"; break;
        case mizu::Key::KpMultiply: repr = "KpMultiply"; break;
        case mizu::Key::KpMinus: repr = "KpMinus"; break;
        case mizu::Key::KpPlus: repr = "KpPlus"; break;
        case mizu::Key::KpEnter: repr = "KpEnter"; break;
        case mizu::Key::Kp1: repr = "Kp1"; break;
        case mizu::Key::Kp2: repr = "Kp2"; break;
        case mizu::Key::Kp3: repr = "Kp3"; break;
        case mizu::Key::Kp4: repr = "Kp4"; break;
        case mizu::Key::Kp5: repr = "Kp5"; break;
        case mizu::Key::Kp6: repr = "Kp6"; break;
        case mizu::Key::Kp7: repr = "Kp7"; break;
        case mizu::Key::Kp8: repr = "Kp8"; break;
        case mizu::Key::Kp9: repr = "Kp9"; break;
        case mizu::Key::Kp0: repr = "Kp0"; break;
        case mizu::Key::KpPeriod: repr = "KpPeriod"; break;
        case mizu::Key::Application: repr = "Application"; break;
        case mizu::Key::Power: repr = "Power"; break;
        case mizu::Key::KpEquals: repr = "KpEquals"; break;
        case mizu::Key::F13: repr = "F13"; break;
        case mizu::Key::F14: repr = "F14"; break;
        case mizu::Key::F15: repr = "F15"; break;
        case mizu::Key::F16: repr = "F16"; break;
        case mizu::Key::F17: repr = "F17"; break;
        case mizu::Key::F18: repr = "F18"; break;
        case mizu::Key::F19: repr = "F19"; break;
        case mizu::Key::F20: repr = "F20"; break;
        case mizu::Key::F21: repr = "F21"; break;
        case mizu::Key::F22: repr = "F22"; break;
        case mizu::Key::F23: repr = "F23"; break;
        case mizu::Key::F24: repr = "F24"; break;
        case mizu::Key::Execute: repr = "Execute"; break;
        case mizu::Key::Help: repr = "Help"; break;
        case mizu::Key::Menu: repr = "Menu"; break;
        case mizu::Key::Select: repr = "Select"; break;
        case mizu::Key::Stop: repr = "Stop"; break;
        case mizu::Key::Again: repr = "Again"; break;
        case mizu::Key::Undo: repr = "Undo"; break;
        case mizu::Key::Cut: repr = "Cut"; break;
        case mizu::Key::Copy: repr = "Copy"; break;
        case mizu::Key::Paste: repr = "Paste"; break;
        case mizu::Key::Find: repr = "Find"; break;
        case mizu::Key::Mute: repr = "Mute"; break;
        case mizu::Key::VolumeUp: repr = "VolumeUp"; break;
        case mizu::Key::VolumeDown: repr = "VolumeDown"; break;
        case mizu::Key::KpComma: repr = "UpComma"; break;
        case mizu::Key::KpEqualsAs400: repr = "UpEqualsAs400"; break;
        case mizu::Key::AltErase: repr = "AltErase"; break;
        case mizu::Key::SysReq: repr = "SysReq"; break;
        case mizu::Key::Cancel: repr = "Cancel"; break;
        case mizu::Key::Clear: repr = "Clear"; break;
        case mizu::Key::Prior: repr = "Prior"; break;
        case mizu::Key::Return2: repr = "Return2"; break;
        case mizu::Key::Separator: repr = "Separator"; break;
        case mizu::Key::Out: repr = "Out"; break;
        case mizu::Key::Oper: repr = "Oper"; break;
        case mizu::Key::ClearAgain: repr = "ClearAgain"; break;
        case mizu::Key::CrSel: repr = "CrSel"; break;
        case mizu::Key::ExSel: repr = "ExSel"; break;
        case mizu::Key::Kp00: repr = "Kp00"; break;
        case mizu::Key::Kp000: repr = "Kp000"; break;
        case mizu::Key::ThousandsSeparator: repr = "ThousandsSeparator"; break;
        case mizu::Key::DecimalSeparator: repr = "DecimalSeparator"; break;
        case mizu::Key::CurrencyUnit: repr = "CurrencyUnit"; break;
        case mizu::Key::CurrencySubUnit: repr = "CurrencySubUnit"; break;
        case mizu::Key::KpLeftParen: repr = "Kp("; break;
        case mizu::Key::KpRightParen: repr = "Kp)"; break;
        case mizu::Key::KpLeftBrace: repr = "Kp["; break;
        case mizu::Key::KpRightBrace: repr = "Kp]"; break;
        case mizu::Key::KpTab: repr = "KpTab"; break;
        case mizu::Key::KpBackspace: repr = "KpBackspace"; break;
        case mizu::Key::KpA: repr = "KpA"; break;
        case mizu::Key::KpB: repr = "KpB"; break;
        case mizu::Key::KpC: repr = "KpC"; break;
        case mizu::Key::KpD: repr = "KpD"; break;
        case mizu::Key::KpE: repr = "KpE"; break;
        case mizu::Key::KpF: repr = "KpF"; break;
        case mizu::Key::KpXor: repr = "KpXor"; break;
        case mizu::Key::KpPower: repr = "KpPower"; break;
        case mizu::Key::KpPercent: repr = "KpPercent"; break;
        case mizu::Key::KpLess: repr = "KpLess"; break;
        case mizu::Key::KpGreater: repr = "KpGreater"; break;
        case mizu::Key::KpAmpersand: repr = "KpAmpersand"; break;
        case mizu::Key::KpDblAmpersand: repr = "KpDblAmpersand"; break;
        case mizu::Key::KpVerticalBar: repr = "KpVerticalBar"; break;
        case mizu::Key::KpDblVerticalBar: repr = "KpDblVerticalBar"; break;
        case mizu::Key::KpColon: repr = "KpColon"; break;
        case mizu::Key::KpHash: repr = "KpHash"; break;
        case mizu::Key::KpSpace: repr = "KpSpace"; break;
        case mizu::Key::KpAt: repr = "KpAt"; break;
        case mizu::Key::KpExclam: repr = "KpExclam"; break;
        case mizu::Key::KpMemStore: repr = "KpMemStore"; break;
        case mizu::Key::KpMemRecall: repr = "KpMemRecall"; break;
        case mizu::Key::KpMemClear: repr = "KpMemClear"; break;
        case mizu::Key::KpMemAdd: repr = "KpMemAdd"; break;
        case mizu::Key::KpMemSubtract: repr = "KpMemSubtract"; break;
        case mizu::Key::KpMemMultiply: repr = "KpMemMultiply"; break;
        case mizu::Key::KpMemDivide: repr = "KpMemDivide"; break;
        case mizu::Key::KpPlusMinus: repr = "KpPlusMinus"; break;
        case mizu::Key::KpClear: repr = "KpClear"; break;
        case mizu::Key::KpClearEntry: repr = "KpClearEntry"; break;
        case mizu::Key::KpBinary: repr = "KpBinary"; break;
        case mizu::Key::KpOctal: repr = "KpOctal"; break;
        case mizu::Key::KpDecimal: repr = "KpDecimal"; break;
        case mizu::Key::KpHexadecimal: repr = "KpHexadecimal"; break;
        case mizu::Key::LCtrl: repr = "LCtrl"; break;
        case mizu::Key::LShift: repr = "LShift"; break;
        case mizu::Key::LAlt: repr = "LAlt"; break;
        case mizu::Key::LGui: repr = "LGui"; break;
        case mizu::Key::RCtrl: repr = "RCtrl"; break;
        case mizu::Key::RShift: repr = "RShift"; break;
        case mizu::Key::RAlt: repr = "RAlt"; break;
        case mizu::Key::RGui: repr = "RGui"; break;
        case mizu::Key::Mode: repr = "Mode"; break;
        case mizu::Key::Sleep: repr = "Sleep"; break;
        case mizu::Key::Wake: repr = "Wake"; break;
        case mizu::Key::ChannelIncrement: repr = "ChannelIncrement"; break;
        case mizu::Key::ChannelDecrement: repr = "ChannelDecrement"; break;
        case mizu::Key::MediaPlay: repr = "MediaPlay"; break;
        case mizu::Key::MediaPause: repr = "MediaPause"; break;
        case mizu::Key::MediaRecord: repr = "MediaRecord"; break;
        case mizu::Key::MediaFastForward: repr = "MediaFastForward"; break;
        case mizu::Key::MediaRewind: repr = "MediaRewind"; break;
        case mizu::Key::MediaNextTrack: repr = "MediaNextTrack"; break;
        case mizu::Key::MediaPreviousTrack: repr = "MediaPreviousTrack"; break;
        case mizu::Key::MediaStop: repr = "MediaStop"; break;
        case mizu::Key::MediaEject: repr = "MediaEject"; break;
        case mizu::Key::MediaPlayPause: repr = "MediaPlayPause"; break;
        case mizu::Key::MediaSelect: repr = "MediaSelect"; break;
        case mizu::Key::AcNew: repr = "AcNew"; break;
        case mizu::Key::AcOpen: repr = "AcOpen"; break;
        case mizu::Key::AcClose: repr = "AcClose"; break;
        case mizu::Key::AcExit: repr = "AcExit"; break;
        case mizu::Key::AcSave: repr = "AcSave"; break;
        case mizu::Key::AcPrint: repr = "AcPrint"; break;
        case mizu::Key::AcProperties: repr = "AcProperties"; break;
        case mizu::Key::AcSearch: repr = "AcSearch"; break;
        case mizu::Key::AcHome: repr = "AcHome"; break;
        case mizu::Key::AcBack: repr = "AcBack"; break;
        case mizu::Key::AcForward: repr = "AcForward"; break;
        case mizu::Key::AcStop: repr = "AcStop"; break;
        case mizu::Key::AcRefresh: repr = "AcRefresh"; break;
        case mizu::Key::AcBookmarks: repr = "AcBookmarks"; break;
        case mizu::Key::SoftLeft: repr = "SoftLeft"; break;
        case mizu::Key::SoftRight: repr = "SoftRight"; break;
        case mizu::Key::Call: repr = "Call"; break;
        case mizu::Key::EndCall: repr = "EndCall"; break;
        case mizu::Key::LeftTab: repr = "LeftTab"; break;
        case mizu::Key::Level5Shift: repr = "Level5Shift"; break;
        case mizu::Key::MultiKeyCompose: repr = "MultiKeyCompose"; break;
        case mizu::Key::LMeta: repr = "LMeta"; break;
        case mizu::Key::RMeta: repr = "RMeta"; break;
        case mizu::Key::LHyper: repr = "LHyper"; break;
        case mizu::Key::RHyper: repr = "RHyper"; break;
        }
        return formatter<string_view>::format(repr, ctx);
    }
};

template<>
struct fmt::formatter<mizu::MouseButton> : formatter<string_view> {
    auto format(mizu::MouseButton b, format_context &ctx) const -> format_context::iterator {
        string_view repr;
        switch (b) {
        case mizu::MouseButton::Left: repr = "Left"; break;
        case mizu::MouseButton::Middle: repr = "Middle"; break;
        case mizu::MouseButton::Right: repr = "Right"; break;
        case mizu::MouseButton::X1: repr = "X1"; break;
        case mizu::MouseButton::X2: repr = "X2"; break;
        }
        return formatter<string_view>::format(repr, ctx);
    }
};

template<>
struct fmt::formatter<mizu::Mod> : formatter<string_view> {
    auto format(mizu::Mod m, format_context &ctx) const -> format_context::iterator {
        if (m == mizu::Mod::None)
            return formatter<string_view>::format("None", ctx);

        std::string buf;
        if (is_flag_set(m, mizu::Mod::Num))
            buf.append("Num");
        if (is_flag_set(m, mizu::Mod::Caps))
            buf.append(fmt::format("{}Caps", buf.empty() ? "" : " | "));
        if (is_flag_set(m, mizu::Mod::Scroll))
            buf.append(fmt::format("{}Scroll", buf.empty() ? "" : " | "));
        if (is_flag_set(m, mizu::Mod::LShift))
            buf.append(fmt::format("{}LShift", buf.empty() ? "" : " | "));
        if (is_flag_set(m, mizu::Mod::RShift))
            buf.append(fmt::format("{}RShift", buf.empty() ? "" : " | "));
        if (is_flag_set(m, mizu::Mod::Level5))
            buf.append(fmt::format("{}Level5", buf.empty() ? "" : " | "));
        if (is_flag_set(m, mizu::Mod::LCtrl))
            buf.append(fmt::format("{}LCtrl", buf.empty() ? "" : " | "));
        if (is_flag_set(m, mizu::Mod::RCtrl))
            buf.append(fmt::format("{}RCtrl", buf.empty() ? "" : " | "));
        if (is_flag_set(m, mizu::Mod::LAlt))
            buf.append(fmt::format("{}LAlt", buf.empty() ? "" : " | "));
        if (is_flag_set(m, mizu::Mod::RAlt))
            buf.append(fmt::format("{}RAlt", buf.empty() ? "" : " | "));
        if (is_flag_set(m, mizu::Mod::LGui))
            buf.append(fmt::format("{}LGui", buf.empty() ? "" : " | "));
        if (is_flag_set(m, mizu::Mod::RGui))
            buf.append(fmt::format("{}RGui", buf.empty() ? "" : " | "));
        return formatter<string_view>::format(buf, ctx);
    }
};

#endif // MIZUINPUT_TYPES_HPP
