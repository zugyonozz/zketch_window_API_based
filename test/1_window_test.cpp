#include <windows.h>
#include <iostream>
#include <vector>
#include <cmath>

// Include semua header z framework
#include "../include/z_window.h"
#include "../include/z_canvas.h"
#include "../include/z_event.h"
#include "../include/z_timer.h"
#include "../include/z_unit.h"

using namespace z;

// Struktur untuk menyimpan partikel sederhana
struct Particle {
    Vec2<float> position;
    Vec2<float> velocity;
    Color<unsigned char> color;
    float life;
    
    Particle(Vec2<float> pos, Vec2<float> vel, Color<unsigned char> col) 
        : position(pos), velocity(vel), color(col), life(1.0f) {}
    
    void update(float dt) {
        position = position + velocity * dt;
        life -= dt * 0.5f; // Partikel hilang dalam 2 detik
        
        // Fade out
        color.a = static_cast<unsigned char>(life * 255);
    }
    
    bool isAlive() const {
        return life > 0;
    }
};

int main() {
    try {
        // 1. Membuat window menggunakan z_unit types
        Vec2<int> windowSize(800, 600);
        Window window("Z Framework Demo", windowSize);
        
        // 2. Membuat canvas untuk drawing
        Canvas canvas(window.handle());
        
        // 3. Setup timer untuk frame rate control
        Timer timer(TimerMode::Precise);
        
        // 4. Show window
        window.show();
        window.centerOnScreen();
        
        // Variables untuk demo
        std::vector<Particle> particles;
        Vec2<int> mousePos(0, 0);
        bool mousePressed = false;
        
        // Demo shapes
        Vec2<int> circleCenter(200, 150);
        int circleRadius = 50;
        
        Rect<int> rectangle(50, 50, 100, 80);
        
        // Colors menggunakan z_unit
        Color<unsigned char> bgColor(20, 20, 30, 255);
        Color<unsigned char> circleColor(100, 150, 255, 255);
        Color<unsigned char> rectColor(255, 100, 100, 255);
        Color<unsigned char> textColor(255, 255, 255, 255);
        
        std::cout << "Z Framework Demo Started!" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "- Click and drag to create particles" << std::endl;
        std::cout << "- ESC to exit" << std::endl;
        std::cout << "- SPACE to clear particles" << std::endl;
        
        // Main loop
        while (!window.shouldClose()) {
            timer.tick();
            float deltaTime = timer.deltaTime();
            
            // Process Windows messages
            window.processMessages();
            
            // Handle events
            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case EventType::Quit:
                        window.close();
                        break;
                        
                    case EventType::KeyDown:
                        if (event.key.keyCode == VK_ESCAPE) {
                            window.close();
                        }
                        if (event.key.keyCode == VK_SPACE) {
                            particles.clear();
                        }
                        break;
                        
                    case EventType::MouseMove:
                        mousePos = event.getMousePosition();
                        break;
                        
                    case EventType::MouseDown:
                        if (event.mouse.button == MouseButton::Left) {
                            mousePressed = true;
                        }
                        break;
                        
                    case EventType::MouseUp:
                        if (event.mouse.button == MouseButton::Left) {
                            mousePressed = false;
                        }
                        break;
                        
                    case EventType::Resize:
                        canvas.resize();
                        windowSize = event.getResizeSize();
                        break;
                }
            }
            
            // Create particles when mouse is pressed
            if (mousePressed && particles.size() < 500) {
                for (int i = 0; i < 3; i++) {
                    Vec2<float> pos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                    Vec2<float> vel(
                        (rand() % 200 - 100) * 2.0f,
                        (rand() % 200 - 100) * 2.0f
                    );
                    Color<unsigned char> color(
                        rand() % 155 + 100,
                        rand() % 155 + 100,
                        rand() % 155 + 100,
                        255
                    );
                    particles.emplace_back(pos, vel, color);
                }
            }
            
            // Update particles
            for (auto it = particles.begin(); it != particles.end();) {
                it->update(deltaTime);
                if (!it->isAlive()) {
                    it = particles.erase(it);
                } else {
                    ++it;
                }
            }
            
            // Animate circle
            static float time = 0;
            time += deltaTime;
            circleCenter.x = 200 + static_cast<int>(sin(time) * 100);
            circleCenter.y = 150 + static_cast<int>(cos(time * 0.7f) * 50);
            
            // Clear canvas
            canvas.clear(bgColor);
            
            // Draw background grid
            Color<unsigned char> gridColor(40, 40, 50, 255);
            for (int x = 0; x < windowSize.x; x += 50) {
                canvas.drawLine(x, 0, x, windowSize.y, gridColor);
            }
            for (int y = 0; y < windowSize.y; y += 50) {
                canvas.drawLine(0, y, windowSize.x, y, gridColor);
            }
            
            // Draw shapes
            canvas.fillRect(rectangle, rectColor, Color<unsigned char>(200, 200, 200, 255), 2);
            canvas.fillCircle(circleCenter, circleRadius, circleColor, Color<unsigned char>(255, 255, 255, 255), 3);
            
            // Draw particles
            for (const auto& particle : particles) {
                Vec2<int> pos(static_cast<int>(particle.position.x), static_cast<int>(particle.position.y));
                canvas.fillCircle(pos, 3, particle.color);
            }
            
            // Draw mouse cursor
            canvas.drawLine(mousePos.x - 10, mousePos.y, mousePos.x + 10, mousePos.y, 
                           Color<unsigned char>(255, 255, 0, 255), 2);
            canvas.drawLine(mousePos.x, mousePos.y - 10, mousePos.x, mousePos.y + 10, 
                           Color<unsigned char>(255, 255, 0, 255), 2);
            
            // Draw some polygons
            Vec2<int> trianglePoints[] = {
                {windowSize.x - 100, 50},
                {windowSize.x - 50, 50},
                {windowSize.x - 75, 100}
            };
            canvas.fillPolygon(trianglePoints, 3, Color<unsigned char>(255, 255, 0, 255));
            
            // Draw info text using basic pixel drawing (since no text API in headers)
            // This is a simple way to show some info
            Color<unsigned char> infoColor(255, 255, 255, 255);
            
            // Draw simple "FPS" indicator using rectangles
            float fps = 1.0f / deltaTime;
            int fpsBarWidth = static_cast<int>(fps * 2);
            if (fpsBarWidth > 200) fpsBarWidth = 200;
            
            canvas.fillRect(10, 10, 200, 20, Color<unsigned char>(50, 50, 50, 255));
            canvas.fillRect(10, 10, fpsBarWidth, 20, 
                           fps > 30 ? Color<unsigned char>(0, 255, 0, 255) : Color<unsigned char>(255, 0, 0, 255));
            
            // Draw particle count indicator
            int particleBarWidth = static_cast<int>(particles.size() * 0.4f);
            canvas.fillRect(10, 35, 200, 15, Color<unsigned char>(50, 50, 50, 255));
            canvas.fillRect(10, 35, particleBarWidth, 15, Color<unsigned char>(0, 100, 255, 255));
            
            // Present the frame
            canvas.present();
            
            // Control frame rate (60 FPS)
            timer.sleepToFps(60.0f);
        }
        
    } catch (const std::exception& e) {
        MessageBox(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    
    std::cout << "Z Framework Demo Ended!" << std::endl;
    return 0;
}