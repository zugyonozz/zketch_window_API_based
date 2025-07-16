#include <cstdio>
#include "../include/z_window.h"
#include "../include/z_event_util.h"
#include "../include/z_timer.h"

class MyWindow : public z::Window<MyWindow> {
public:
    MyWindow() : z::Window<MyWindow>("Event Window", 800, 600) {}

    LRESULT onEvent(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        z::Event ev = z::translateWinEvent(hwnd, msg, wp, lp) ;

        switch (ev.type) {
            case z::EventType::Quit:
                PostQuitMessage(0) ;
                return 0 ;

            case z::EventType::KeyDown:
                if (ev.key.keyCode == VK_ESCAPE)
                    PostQuitMessage(0) ;
                break ;

            case z::EventType::MouseMove:
                printf("Mouse at (%d, %d)\n", ev.mouse.x, ev.mouse.y) ;
                break ;

            case z::EventType::Resize:
                printf("Resize: %d x %d\n", ev.resize.width, ev.resize.height) ;
                break ;

            default:
                break ;
        }

        return DefWindowProc(hwnd, msg, wp, lp) ;
    }
};

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int cmdShow) {
    MyWindow w;
	z::Timer timer(z::TimerMode::Simple) ;
    w.show(cmdShow);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
		timer.sleepToFps(60) ;
    }

    return 0;
}