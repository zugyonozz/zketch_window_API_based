#pragma once
#include "z_event.h"
#include "z_unit.h"

namespace z {

// Fungsi konversi WinAPI ke Event
inline Event translateWinEvent(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    Event ev;

    switch (msg) {
        case WM_QUIT:
        case WM_CLOSE:
        case WM_DESTROY:
            ev.type = EventType::Quit;
            break;

        case WM_KEYDOWN:
            ev.type = EventType::KeyDown;
            ev.key.keyCode = (int)wp;
            break;

        case WM_KEYUP:
            ev.type = EventType::KeyUp;
            ev.key.keyCode = (int)wp;
            break;

        case WM_MOUSEMOVE:
            ev.type = EventType::MouseMove;
            ev.mouse.x = GET_X_LPARAM(lp);
            ev.mouse.y = GET_Y_LPARAM(lp);
            ev.mouse.button = MouseButton::Unknown;
            break;

        case WM_LBUTTONDOWN:
            ev.type = EventType::MouseDown;
            ev.mouse.x = GET_X_LPARAM(lp);
            ev.mouse.y = GET_Y_LPARAM(lp);
            ev.mouse.button = MouseButton::Left;
            break;

        case WM_RBUTTONDOWN:
            ev.type = EventType::MouseDown;
            ev.mouse.x = GET_X_LPARAM(lp);
            ev.mouse.y = GET_Y_LPARAM(lp);
            ev.mouse.button = MouseButton::Right;
            break;

        case WM_LBUTTONUP:
            ev.type = EventType::MouseUp;
            ev.mouse.x = GET_X_LPARAM(lp);
            ev.mouse.y = GET_Y_LPARAM(lp);
            ev.mouse.button = MouseButton::Left;
            break;

        case WM_RBUTTONUP:
            ev.type = EventType::MouseUp;
            ev.mouse.x = GET_X_LPARAM(lp);
            ev.mouse.y = GET_Y_LPARAM(lp);
            ev.mouse.button = MouseButton::Right;
            break;

        case WM_SIZE:
            ev.type = EventType::Resize;
            ev.resize.width = LOWORD(lp);
            ev.resize.height = HIWORD(lp);
            break;

        default:
            ev.type = EventType::None;
            break;
    }

    return ev;
}

// Helper functions for working with Events and z_unit types
inline Vec2<int> getEventPosition(const Event& event) {
    return event.getMousePosition();
}

inline Vec2<int> getEventSize(const Event& event) {
    return event.getResizeSize();
}

// Create mouse event with Vec2 position
inline Event createMouseEvent(EventType type, Vec2<int> position, MouseButton button) {
    Event event;
    event.type = type;
    event.mouse.x = position.x;
    event.mouse.y = position.y;
    event.mouse.button = button;
    return event;
}

// Create resize event with Vec2 size
inline Event createResizeEvent(Vec2<int> size) {
    Event event;
    event.type = EventType::Resize;
    event.resize.width = size.x;
    event.resize.height = size.y;
    return event;
}

// Create key event
inline Event createKeyEvent(EventType type, int keyCode) {
    Event event;
    event.type = type;
    event.key.keyCode = keyCode;
    return event;
}

}