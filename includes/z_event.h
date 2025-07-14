#pragma once
#include <windows.h>
#include <windowsx.h>

namespace z {

enum class EventType {
    None,
    Quit,
    KeyDown,
    KeyUp,
    MouseMove,
    MouseDown,
    MouseUp,
    Resize
};

enum class MouseButton {
    Left,
    Right,
    Middle,
    Unknown
};

struct Event {
    EventType type = EventType::None;

    union {
        struct { int keyCode; } key;
        struct { int x, y; MouseButton button; } mouse;
        struct { int width, height; } resize;
    };
};

}