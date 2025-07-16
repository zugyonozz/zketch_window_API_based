#pragma once
#include <windows.h>
#include <windowsx.h>
#include "z_unit.h"

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
        struct { 
            int x, y; 
            MouseButton button;
            
            // Helper methods to get Vec2
            Vec2<int> position() const { return Vec2<int>(x, y); }
            void setPosition(Vec2<int> pos) { x = pos.x; y = pos.y; }
        } mouse;
        struct { 
            int width, height;
            
            // Helper methods to get Vec2
            Vec2<int> size() const { return Vec2<int>(width, height); }
            void setSize(Vec2<int> newSize) { width = newSize.x; height = newSize.y; }
        } resize;
    };

    // Convenience methods for mouse events
    Vec2<int> getMousePosition() const {
        return (type == EventType::MouseMove || type == EventType::MouseDown || type == EventType::MouseUp) ? mouse.position() : Vec2<int>(0, 0);
    }

    void setMousePosition(Vec2<int> pos) {
        if (type == EventType::MouseMove || type == EventType::MouseDown || type == EventType::MouseUp) {
            mouse.setPosition(pos);
        }
    }

    // Convenience methods for resize events
    Vec2<int> getResizeSize() const {
        return (type == EventType::Resize) ? resize.size() : Vec2<int>(0, 0);
    }

    void setResizeSize(Vec2<int> newSize) {
        if (type == EventType::Resize) {
            resize.setSize(newSize);
        }
    }

    // Check if event is mouse-related
    bool isMouseEvent() const {
        return type == EventType::MouseMove || type == EventType::MouseDown || type == EventType::MouseUp;
    }

    // Check if event is keyboard-related
    bool isKeyEvent() const {
        return type == EventType::KeyDown || type == EventType::KeyUp;
    }

    // Check if mouse button matches
    bool isMouseButton(MouseButton button) const {
        return isMouseEvent() && mouse.button == button;
    }

    // Check if key matches
    bool isKey(int keyCode) const {
        return isKeyEvent() && key.keyCode == keyCode;
    }
};

}