#pragma once
#include <windows.h>

namespace z {

class Canvas {
public:
    Canvas(HWND hwnd)
        : m_hwnd(hwnd) {
        m_hdc = GetDC(hwnd);
        m_pen   = CreatePen(PS_SOLID, 1, RGB(0,0,0));
        m_brush = CreateSolidBrush(RGB(255,255,255));
        SelectObject(m_hdc, m_pen);
        SelectObject(m_hdc, m_brush);
    }

    ~Canvas() {
        DeleteObject(m_pen);
        DeleteObject(m_brush);
        ReleaseDC(m_hwnd, m_hdc);
    }

    void setColor(COLORREF color) {
        DeleteObject(m_pen);
        DeleteObject(m_brush);
        m_pen   = CreatePen(PS_SOLID, 1, color);
        m_brush = CreateSolidBrush(color);
        SelectObject(m_hdc, m_pen);
        SelectObject(m_hdc, m_brush);
    }

    void clear(COLORREF color) {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        HBRUSH bg = CreateSolidBrush(color);
        FillRect(m_hdc, &rect, bg);
        DeleteObject(bg);
    }

    void fillRect(int x, int y, int w, int h) {
        Rectangle(m_hdc, x, y, x + w, y + h);
    }

    void drawRect(int x, int y, int w, int h) {
        HBRUSH oldBrush = (HBRUSH)SelectObject(m_hdc, GetStockObject(NULL_BRUSH));
        Rectangle(m_hdc, x, y, x + w, y + h);
        SelectObject(m_hdc, oldBrush);
    }

private:
    HWND m_hwnd;
    HDC  m_hdc;
    HPEN m_pen;
    HBRUSH m_brush;
};

}