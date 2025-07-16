#include "../includes/z_window.h"
#include "../includes/z_canvas.h"
#include "../includes/z_timer.h"
#include "../includes/z_event_util.h"

class GameWindow : public z::Window<GameWindow> {
public:
    GameWindow() : z::Window<GameWindow>("Canvas Demo - SDL3 Style", 800, 600) {
        canvas = std::make_unique<z::Canvas>(handle());
        timer = std::make_unique<z::Timer>(z::TimerMode::Precise);
        running = true;
    }

    LRESULT onEvent(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        z::Event event = z::translateWinEvent(hwnd, msg, wp, lp);
        
        switch (event.type) {
            case z::EventType::Quit:
                running = false;
                PostQuitMessage(0);
                return 0;
                
            case z::EventType::Resize:
                canvas->resize();
                break;
                
            case z::EventType::KeyDown:
                if (event.key.keyCode == VK_ESCAPE) {
                    running = false;
                    PostQuitMessage(0);
                }
                break;
                
            case z::EventType::MouseDown:
                if (event.mouse.button == z::MouseButton::Left) {
                    mouseX = event.mouse.x;
                    mouseY = event.mouse.y;
                }
                break;
        }
        
        return DefWindowProc(hwnd, msg, wp, lp);
    }

    void run() {
        show(SW_SHOWDEFAULT);
        
        MSG msg = {};
        while (running) {
            timer->tick();
            
            // Handle messages
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            
            if (!running) break;
            
            update();
            render();
            
            timer->sleepToFps(60.0f);
        }
    }

private:
    std::unique_ptr<z::Canvas> canvas;
    std::unique_ptr<z::Timer> timer;
    bool running;
    int mouseX = 0, mouseY = 0;
    float time = 0.0f;

    void update() {
        time += timer->deltaTime();
    }

    void render() {
        // Clear screen
        canvas->clear(RGB(20, 20, 30));
        
        // === DEMO 1: Basic rectangle (outline only) ===
        canvas->drawRect(50, 50, 100, 80);
        
        // === DEMO 2: Rectangle with custom stroke ===
        canvas->drawRect(200, 50, 100, 80, RGB(255, 0, 0), 3);
        
        // === DEMO 3: Rectangle with fill only ===
        canvas->drawRect(350, 50, 100, 80, RGB(0, 255, 0));
        
        // === DEMO 4: Rectangle with fill and stroke ===
        canvas->drawRect(500, 50, 100, 80, RGB(0, 0, 255), RGB(255, 255, 0), 2);
        
        // === DEMO 5: Multiple rectangles dengan warna berbeda ===
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 5; y++) {
                COLORREF color = RGB(
                    static_cast<int>(127 + 127 * sin(time + x * 0.5f)),
                    static_cast<int>(127 + 127 * sin(time + y * 0.3f)),
                    static_cast<int>(127 + 127 * sin(time + (x + y) * 0.2f))
                );
                canvas->drawRect(50 + x * 35, 200 + y * 35, 30, 30, color);
            }
        }
        
        // === DEMO 6: Interactive rectangle at mouse position ===
        canvas->drawRect(mouseX - 25, mouseY - 25, 50, 50, RGB(255, 200, 100), RGB(255, 255, 255), 2);
        
        // === DEMO 7: Circle variations ===
        canvas->drawCircle(650, 250, 40, RGB(255, 255, 255), 2);  // outline only
        canvas->drawCircle(650, 350, 30, RGB(100, 255, 255)) ;      // fill only
        canvas->drawCircle(650, 450, 35, RGB(255, 100, 255), RGB(255, 255, 255), 3); // fill + stroke
        
        // === DEMO 8: Lines ===
        for (int i = 0; i < 20; i++) {
            canvas->drawLine(
                50 + i * 20, 450,
                50 + i * 20, static_cast<int>(450 + 50 * sin(time * 2 + i * 0.5f)),
                RGB(255, 255, 255), 1
            );
        }
        
        // === DEMO 9: Pixels ===
        for (int i = 0; i < 100; i++) {
            int px = 150 + static_cast<int>(40 * cos(time + i * 0.1f));
            int py = 500 + static_cast<int>(40 * sin(time + i * 0.1f));
            COLORREF pixelColor = RGB(
                static_cast<int>(255 * (sin(time + i * 0.1f) + 1) / 2),
                static_cast<int>(255 * (cos(time + i * 0.1f) + 1) / 2),
                128
            );
            canvas->drawPixel(px, py, pixelColor);
        }
        
        // Present the final frame
        canvas->present();
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    try {
        GameWindow window;
        window.run();
    } catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    
    return 0;
}