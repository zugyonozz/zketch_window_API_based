#pragma once
#include <windows.h>
#include <string>
#include <stdexcept>
#include <functional>
#include <queue>
#include "z_event.h"
#include "z_event_util.h"
#include "z_unit.h"

namespace z {

class Window {
public:
    // Constructor - simple and straightforward
    Window(const char* title, int width, int height) 
        : m_size(width, height), m_title(title) {
        m_hInstance = GetModuleHandle(nullptr);
        registerWindowClass();
        createWindow();
        
        // Set this pointer untuk callback
        SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
    }

    // Constructor with Vec2 size
    Window(const char* title, Vec2<int> size) 
        : m_size(size), m_title(title) {
        m_hInstance = GetModuleHandle(nullptr);
        registerWindowClass();
        createWindow();
        
        SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
    }

    // Constructor with Rect (position + size)
    Window(const char* title, Rect<int> bounds) 
        : m_size(bounds.w, bounds.h), m_position(bounds.x, bounds.y), m_title(title) {
        m_hInstance = GetModuleHandle(nullptr);
        registerWindowClass();
        createWindow();
        
        SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
    }

    // Destructor
    ~Window() {
        destroy();
    }

    // Delete copy constructor and assignment (modern C++ best practice)
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Move constructor and assignment
    Window(Window&& other) noexcept 
        : m_hwnd(other.m_hwnd), m_hInstance(other.m_hInstance), 
          m_size(other.m_size), m_position(other.m_position),
          m_title(std::move(other.m_title)) {
        other.m_hwnd = nullptr;
        other.m_hInstance = nullptr;
        if (m_hwnd) {
            SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
        }
    }

    Window& operator=(Window&& other) noexcept {
        if (this != &other) {
            destroy();
            m_hwnd = other.m_hwnd;
            m_hInstance = other.m_hInstance;
            m_size = other.m_size;
            m_position = other.m_position;
            m_title = std::move(other.m_title);
            
            other.m_hwnd = nullptr;
            other.m_hInstance = nullptr;
            
            if (m_hwnd) {
                SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
            }
        }
        return *this;
    }

    // Show window - SDL3 style
    void show() {
        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);
    }

    void hide() {
        ShowWindow(m_hwnd, SW_HIDE);
    }

    void minimize() {
        ShowWindow(m_hwnd, SW_MINIMIZE);
    }

    void maximize() {
        ShowWindow(m_hwnd, SW_MAXIMIZE);
    }

    void restore() {
        ShowWindow(m_hwnd, SW_RESTORE);
    }

    // Get window handle
    HWND handle() const {
        return m_hwnd;
    }

    // Get window properties - legacy methods
    int width() const { return m_size.x; }
    int height() const { return m_size.y; }
    const std::string& title() const { return m_title; }

    // Get window properties - z_unit methods
    Vec2<int> size() const { return m_size; }
    Vec2<int> position() const { return m_position; }
    Rect<int> bounds() const { return Rect<int>(m_position.x, m_position.y, m_size.x, m_size.y); }

    // Set window properties - legacy methods
    void setTitle(const char* title) {
        m_title = title;
        SetWindowText(m_hwnd, title);
    }

    void setSize(int width, int height) {
        setSize(Vec2<int>(width, height));
    }

    void setPosition(int x, int y) {
        setPosition(Vec2<int>(x, y));
    }

    // Set window properties - z_unit methods
    void setSize(Vec2<int> newSize) {
        m_size = newSize;
        SetWindowPos(m_hwnd, nullptr, 0, 0, m_size.x, m_size.y, 
                    SWP_NOMOVE | SWP_NOZORDER);
    }

    void setPosition(Vec2<int> newPosition) {
        m_position = newPosition;
        SetWindowPos(m_hwnd, nullptr, m_position.x, m_position.y, 0, 0, 
                    SWP_NOSIZE | SWP_NOZORDER);
    }

    void setBounds(Rect<int> newBounds) {
        m_size = Vec2<int>(newBounds.w, newBounds.h);
        m_position = Vec2<int>(newBounds.x, newBounds.y);
        SetWindowPos(m_hwnd, nullptr, m_position.x, m_position.y, 
                    m_size.x, m_size.y, SWP_NOZORDER);
    }

    // Event handling - SDL3 style
    bool pollEvent(Event& event) {
        if (!m_eventQueue.empty()) {
            event = m_eventQueue.front();
            m_eventQueue.pop();
            return true;
        }
        return false;
    }

    // Process Windows messages
    void processMessages() {
        MSG msg;
        while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Check if window should close
    bool shouldClose() const {
        return m_shouldClose;
    }

    // Force close
    void close() {
        m_shouldClose = true;
    }

    // Check if window is valid
    bool isValid() const {
        return m_hwnd != nullptr && IsWindow(m_hwnd);
    }

    // Get client area size - legacy method
    void getClientSize(int& width, int& height) const {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }

    // Get client area size - z_unit method
    Vec2<int> getClientSize() const {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        return Vec2<int>(rect.right - rect.left, rect.bottom - rect.top);
    }

    // Get client area bounds
    Rect<int> getClientBounds() const {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        return Rect<int>(0, 0, static_cast<int>(rect.right - rect.left), static_cast<int>(rect.bottom - rect.top));
    }

    // Center window on screen
    void centerOnScreen() {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        
        RECT rect;
        GetWindowRect(m_hwnd, &rect);
        int windowWidth = rect.right - rect.left;
        int windowHeight = rect.bottom - rect.top;
        
        Vec2<int> centerPos(
            (screenWidth - windowWidth) / 2,
            (screenHeight - windowHeight) / 2
        );
        
        setPosition(centerPos);
    }

    // Check if point is inside client area
    bool containsPoint(Vec2<int> point) const {
        Rect<int> clientBounds = getClientBounds();
        return point.x >= clientBounds.x && point.x < clientBounds.x + clientBounds.w &&
               point.y >= clientBounds.y && point.y < clientBounds.y + clientBounds.h;
    }

    // Convert screen coordinates to client coordinates
    Vec2<int> screenToClient(Vec2<int> screenPos) const {
        POINT pt = {screenPos.x, screenPos.y};
        ScreenToClient(m_hwnd, &pt);
        return Vec2<int>(pt.x, pt.y);
    }

    // Convert client coordinates to screen coordinates
    Vec2<int> clientToScreen(Vec2<int> clientPos) const {
        POINT pt = {clientPos.x, clientPos.y};
        ClientToScreen(m_hwnd, &pt);
        return Vec2<int>(pt.x, pt.y);
    }

private:
    HWND m_hwnd = nullptr;
    HINSTANCE m_hInstance = nullptr;
    Vec2<int> m_size;
    Vec2<int> m_position;
    std::string m_title;
    bool m_shouldClose = false;
    std::queue<Event> m_eventQueue;

    static constexpr const char* CLASS_NAME = "z_Window";

    // Static window procedure
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        Window* window = nullptr;

        if (msg == WM_NCCREATE) {
            CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lp);
            window = reinterpret_cast<Window*>(cs->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
        } else {
            window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (window) {
            return window->handleMessage(hwnd, msg, wp, lp);
        }

        return DefWindowProc(hwnd, msg, wp, lp);
    }

    // Instance window procedure
    LRESULT handleMessage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        // Convert Windows message to our Event and add to queue
        Event event = translateWinEvent(hwnd, msg, wp, lp);
        if (event.type != EventType::None) {
            m_eventQueue.push(event);
        }

        // Handle special cases
        switch (msg) {
            case WM_CLOSE:
                m_shouldClose = true;
                return 0;

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

            case WM_SIZE:
                m_size = Vec2<int>(LOWORD(lp), HIWORD(lp));
                break;

            case WM_MOVE:
                m_position = Vec2<int>(LOWORD(lp), HIWORD(lp));
                break;

            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                // Default: paint black background
                RECT rect;
                GetClientRect(hwnd, &rect);
                FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
                EndPaint(hwnd, &ps);
                return 0;
            }
        }

        return DefWindowProc(hwnd, msg, wp, lp);
    }

    void registerWindowClass() {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = StaticWndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = m_hInstance;
        wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = CLASS_NAME;
        wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

        if (!RegisterClassEx(&wc)) {
            DWORD error = GetLastError();
            if (error != ERROR_CLASS_ALREADY_EXISTS) {
                throw std::runtime_error("Failed to register window class. Error: " + std::to_string(error));
            }
        }
    }

    void createWindow() {
        // Calculate window size to get desired client area
        RECT rect = { 0, 0, m_size.x, m_size.y };
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
        
        int windowWidth = rect.right - rect.left;
        int windowHeight = rect.bottom - rect.top;

        // Use position if set, otherwise use default
        int x = (m_position.x != 0) ? m_position.x : CW_USEDEFAULT;
        int y = (m_position.y != 0) ? m_position.y : CW_USEDEFAULT;

        m_hwnd = CreateWindowEx(
            0,                          // Extended style
            CLASS_NAME,                 // Class name
            m_title.c_str(),           // Window title
            WS_OVERLAPPEDWINDOW,       // Window style
            x,                         // X position
            y,                         // Y position
            windowWidth,               // Width
            windowHeight,              // Height
            nullptr,                   // Parent window
            nullptr,                   // Menu
            m_hInstance,               // Instance handle
            this                       // Additional data
        );

        if (!m_hwnd) {
            DWORD error = GetLastError();
            throw std::runtime_error("Failed to create window. Error: " + std::to_string(error));
        }

        // Update actual position after creation
        if (x == CW_USEDEFAULT || y == CW_USEDEFAULT) {
            RECT winRect;
            GetWindowRect(m_hwnd, &winRect);
            m_position = Vec2<int>(winRect.left, winRect.top);
        }
    }

    void destroy() {
        if (m_hwnd) {
            DestroyWindow(m_hwnd);
            m_hwnd = nullptr;
        }
        if (m_hInstance) {
            UnregisterClass(CLASS_NAME, m_hInstance);
        }
    }
};

} // namespace z