#pragma once
#include <windows.h>
#include <string>
#include <stdexcept>

namespace z {

template <typename Derived>
class Window {
public:
    Window(const char* title, int width, int height) {
		m_hInstance = GetModuleHandle(nullptr) ;
		registerWindowClass() ;
		createWindow(title, width, height) ;

		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this) ;
	}
    ~Window() {
		if (m_hwnd) 
			DestroyWindow(m_hwnd) ;
    	UnregisterClass(m_className.c_str(), m_hInstance) ;
	}

    void show(int nCmdShow) {
		ShowWindow(m_hwnd, nCmdShow) ;
    	UpdateWindow(m_hwnd) ;
	}
    HWND handle() const {
		return m_hwnd ;
	}

protected:
    // Static callback untuk WinAPI
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
		Derived* self = nullptr;

		if (msg == WM_NCCREATE) {
			CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lp);
			self = reinterpret_cast<Derived*>(cs->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)self);
		} else 
			self = reinterpret_cast<Derived*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		if (self) 
			return self->onEvent(hwnd, msg, wp, lp);

		return DefWindowProc(hwnd, msg, wp, lp);
	}

    HWND m_hwnd = nullptr;
    HINSTANCE m_hInstance = nullptr;
    std::string m_className = "z_crtp_window_class";

    void registerWindowClass() {
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = StaticWndProc;
		wc.hInstance = m_hInstance;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszClassName = m_className.c_str();

		if (!RegisterClassEx(&wc))
			throw std::runtime_error("Gagal mendaftarkan kelas jendela.");
	}

    void createWindow(const char* title, int width, int height) {
		m_hwnd = CreateWindowEx(
			0, 
			m_className.c_str(), 
			title, 
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT, 
			width, 
			height, 
			nullptr, 
			nullptr, 
			m_hInstance, 
			this
		);

		if (!m_hwnd)
			throw std::runtime_error("Gagal membuat jendela.");
	}
};

} 