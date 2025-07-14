#include "../includes/z_window.h"

class MyWindow : public z::Window<MyWindow> {
public:
    MyWindow() : z::Window<MyWindow>("CRTP Window", 800, 600) {}

    LRESULT onEvent(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
            case WM_DESTROY:
                PostQuitMessage(0) ;
                return 0 ;
            case WM_KEYDOWN:
                if (wp == VK_ESCAPE) {
                    PostQuitMessage(0) ;
                    return 0 ;
                }
                break ;
        }
        return DefWindowProc(hwnd, msg, wp, lp) ;
    }
};

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int cmdShow) {
    MyWindow w ;
    w.show(cmdShow) ;

    MSG msg = {} ;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg) ;
        DispatchMessage(&msg) ;
    }

    return 0;
}
