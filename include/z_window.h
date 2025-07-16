#pragma once
#include <windows.h>
#include <string>
#include <stdexcept>
#include <functional>
#include <queue>
#include "z_event.h"
#include "z_event_util.h"

namespace z {

class Window {
public:
    // Constructor - simple and straightforward
    Window(const char* title, int width, int height) 
        : m_width(width), m_height(height), m_title(title) {
        m_hInstance = GetModuleHandle(nullptr);
        registerWindowClass();
        createWindow();
        
        // Set this pointer untuk callback
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
          m_width(other.m_width), m_height(other.m_height), 
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
            m_width = other.m_width;
            m_height = other.m_height;
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

    // Get window properties
    int width() const { return m_width; }
    int height() const { return m_height; }
    const std::string& title() const { return m_title; }

    // Set window properties
    void setTitle(const char* title) {
        m_title = title;
        SetWindowText(m_hwnd, title);
    }

    void setSize(int width, int height) {
        m_width = width;
        m_height = height;
        SetWindowPos(m_hwnd, nullptr, 0, 0, width, height, 
                    SWP_NOMOVE | SWP_NOZORDER);
    }

    void setPosition(int x, int y) {
        SetWindowPos(m_hwnd, nullptr, x, y, 0, 0, 
                    SWP_NOSIZE | SWP_NOZORDER);
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

    // Get client area size
    void getClientSize(int& width, int& height) const {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }

    // Center window on screen
    void centerOnScreen() {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        
        RECT rect;
        GetWindowRect(m_hwnd, &rect);
        int windowWidth = rect.right - rect.left;
        int windowHeight = rect.bottom - rect.top;
        
        int x = (screenWidth - windowWidth) / 2;
        int y = (screenHeight - windowHeight) / 2;
        
        setPosition(x, y);
    }

private:
    HWND m_hwnd = nullptr;
    HINSTANCE m_hInstance = nullptr;
    int m_width, m_height;
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
                m_width = LOWORD(lp);
                m_height = HIWORD(lp);
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
        RECT rect = { 0, 0, m_width, m_height };
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
        
        int windowWidth = rect.right - rect.left;
        int windowHeight = rect.bottom - rect.top;

        m_hwnd = CreateWindowEx(
            0,                          // Extended style
            CLASS_NAME,                 // Class name
            m_title.c_str(),           // Window title
            WS_OVERLAPPEDWINDOW,       // Window style
            CW_USEDEFAULT,             // X position
            CW_USEDEFAULT,             // Y position
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