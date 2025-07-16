#include "../include/z_window.h"
#include "../include/z_canvas.h"
#include "../include/z_timer.h"
#include "../include/z_unit.h"
#include <iostream>

using namespace z;

int main() {
    try {
        // Create window using Vec2 for size
        Vec2<int> windowSize(800, 600);
        Window window("Z Engine - Unit Integration Demo", windowSize);
        
        // Alternative: Create window with bounds (position + size)
        // Rect<int> windowBounds(100, 100, 800, 600);
        // Window window("Z Engine Demo", windowBounds);
        
        window.show();
        window.centerOnScreen();
        
        // Create canvas for drawing
        Canvas canvas(window.handle());
        Timer timer;
        
        // Game state
        Vec2<int> playerPos(400, 300);
        Vec2<int> playerSize(50, 50);
        Color<unsigned char> playerColor(255, 100, 100, 255);
        
        // Rectangle for player bounds
        Rect<int> playerBounds(playerPos.x - playerSize.x/2, playerPos.y - playerSize.y/2, 
                              playerSize.x, playerSize.y);
        
        bool running = true;
        
        while (running && !window.shouldClose()) {
            timer.tick();
            
            // Process events
            window.processMessages();
            
            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case EventType::Quit:
                        running = false;
                        break;
                        
                    case EventType::KeyDown:
                        if (event.key.keyCode == VK_ESCAPE) {
                            running = false;
                        }
                        break;
                        
                    case EventType::MouseMove: {
                        // Using Vec2 for mouse position
                        Vec2<int> mousePos = event.getMousePosition();
                        
                        // Convert to client coordinates if needed
                        Vec2<int> clientPos = window.screenToClient(mousePos);
                        
                        std::cout << "Mouse moved to: " << mousePos.x << ", " << mousePos.y << std::endl;
                        break;
                    }
                    
                    case EventType::MouseDown:
                        if (event.mouse.button == MouseButton::Left) {
                            // Move player to mouse position
                            playerPos = event.getMousePosition();
                            
                            // Update player bounds
                            playerBounds = Rect<int>(playerPos.x - playerSize.x/2, 
                                                   playerPos.y - playerSize.y/2, 
                                                   playerSize.x, playerSize.y);
                        }
                        break;
                        
                    case EventType::Resize: {
                        // Handle window resize using Vec2
                        Vec2<int> newSize = event.getResizeSize();
                        canvas.resize();
                        
                        std::cout << "Window resized to: " << newSize.x << "x" << newSize.y << std::endl;
                        break;
                    }
                }
            }
            
            // Update game logic
            Vec2<int> clientSize = window.getClientSize();
            Rect<int> clientBounds = window.getClientBounds();
            
            // Keep player within bounds
            if (playerPos.x < playerSize.x/2) playerPos.x = playerSize.x/2;
            if (playerPos.y < playerSize.y/2) playerPos.y = playerSize.y/2;
            if (playerPos.x > clientSize.x - playerSize.x/2) playerPos.x = clientSize.x - playerSize.x/2;
            if (playerPos.y > clientSize.y - playerSize.y/2) playerPos.y = clientSize.y - playerSize.y/2;
            
            // Update player bounds
            playerBounds = Rect<int>(playerPos.x - playerSize.x/2, 
                                   playerPos.y - playerSize.y/2, 
                                   playerSize.x, playerSize.y);
            
            // Render
            canvas.clear(RGB(30, 30, 30));
            
            // Draw player using Canvas with Rect
            canvas.drawRect(playerBounds, RGB(playerColor.r, playerColor.g, playerColor.b));
            
            // Draw some additional shapes using z_unit types
            Rect<int> borderRect(10, 10, clientSize.x - 20, clientSize.y - 20);
            canvas.drawRect(borderRect, RGB(100, 100, 100), 2);
            
            // Draw crosshair at center
            Vec2<int> center = clientSize / Vec2<int>(2, 2);
            canvas.drawLine(center.x - 10, center.y, center.x + 10, center.y, RGB(255, 255, 255));
            canvas.drawLine(center.x, center.y - 10, center.x, center.y + 10, RGB(255, 255, 255));
            
            canvas.present();
            
            // Control frame rate
            timer.sleepToFps(60.0f);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}

/*
Key improvements with z_unit integration:

1. **Type Safety**: Using Vec2<int> and Rect<int> instead of separate x,y,w,h parameters
2. **Consistent API**: All position/size operations use the same types
3. **Mathematical Operations**: Can use +, -, *, / on Vec2 and Rect types
4. **Convenience Methods**: 
   - window.size() returns Vec2<int>
   - window.getClientBounds() returns Rect<int>
   - event.getMousePosition() returns Vec2<int>
5. **Better Code Readability**: 
   - Vec2<int> playerPos instead of int playerX, playerY
   - Rect<int> bounds instead of x, y, w, h variables
6. **Easier Calculations**:
   - Vec2<int> center = clientSize / Vec2<int>(2, 2)
   - playerPos = mousePos (direct assignment)
   - bounds checking with Rect operations

Usage examples:
- Window creation: Window("title", Vec2<int>(800, 600))
- Window with position: Window("title", Rect<int>(100, 100, 800, 600))
- Getting sizes: Vec2<int> size = window.size()
- Getting bounds: Rect<int> bounds = window.getClientBounds()
- Mouse position: Vec2<int> mousePos = event.getMousePosition()
- Drawing: canvas.drawRect(playerBounds, color)
*/