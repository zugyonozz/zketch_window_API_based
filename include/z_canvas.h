#pragma once
#include <windows.h>
#include <memory>
#include "z_unit.h"

namespace z {

class Canvas {
public:

	enum class Mode : unsigned char {FILL, STROKE, BOTH} ;

    Canvas(HWND hwnd) : m_hwnd(hwnd), m_hdc(nullptr), m_memDC(nullptr), m_memBitmap(nullptr), m_oldBitmap(nullptr) {
        m_hdc = GetDC(hwnd);
        setupDoubleBuffering();
    }

    ~Canvas() {
        cleanup();
        if (m_hdc)
            ReleaseDC(m_hwnd, m_hdc);
    }

    void clear(COLORREF color = RGB(0, 0, 0)) {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        HBRUSH bg = CreateSolidBrush(color);
        FillRect(m_memDC, &rect, bg);
        DeleteObject(bg);
    }

    // Basic rectangle (hanya outline dengan default stroke)
    template <Mode mode = Mode::BOTH> void drawRect(z::Bound<int> bound) {
		if(mode == Mode::STROKE)
        	drawRectInternal(bound, RGB(0,0,0), RGB(255,255,255), false, true, 1);
		else if (mode == Mode::FILL)
		 	drawRectInternal(bound, RGB(0,0,0), RGB(255,255,255), true, false, 1);
		else
			drawRectInternal(bound, RGB(0,0,0), RGB(255,255,255), true, true, 1);
    }

    // Rectangle dengan stroke color dan width
    void drawRect(z::Bound<int> bound, COLORREF strokeColor, int strokeWidth = 1) {
        drawRectInternal(bound, RGB(0,0,0), strokeColor, false, true, strokeWidth);
    }

    // Rectangle dengan fill color saja
    void drawRect(int x, int y, int w, int h, COLORREF fillColor) {
        drawRectInternal(x, y, w, h, fillColor, RGB(0,0,0), true, false, 1);
    }

    // Rectangle dengan fill color dan stroke
    void drawRect(int x, int y, int w, int h, COLORREF fillColor, COLORREF strokeColor, int strokeWidth = 1) {
        drawRectInternal(x, y, w, h, fillColor, strokeColor, true, true, strokeWidth);
    }

    void present() {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        BitBlt(m_hdc, 0, 0, rect.right, rect.bottom, m_memDC, 0, 0, SRCCOPY);
    }

    void resize() {
        cleanup();
        setupDoubleBuffering();
    }

    HDC getHDC() const { return m_memDC; }
    HWND getHWND() const { return m_hwnd; }

    // === ADVANCED DRAWING ===
    void drawLine(int x1, int y1, int x2, int y2, COLORREF color = RGB(255,255,255), int width = 1) {
        HPEN pen = CreatePen(PS_SOLID, width, color);
        HPEN oldPen = (HPEN)SelectObject(m_memDC, pen);
        
        MoveToEx(m_memDC, x1, y1, nullptr);
        LineTo(m_memDC, x2, y2);
        
        SelectObject(m_memDC, oldPen);
        DeleteObject(pen);
    }

    void drawPixel(int x, int y, COLORREF color) {
        SetPixel(m_memDC, x, y, color);
    }

    // Circle dengan outline saja
    void drawCircle(int cx, int cy, int radius, COLORREF strokeColor = RGB(255,255,255), int strokeWidth = 1) {
        HPEN pen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
        HPEN oldPen = (HPEN)SelectObject(m_memDC, pen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(m_memDC, GetStockObject(NULL_BRUSH));
        
        Ellipse(m_memDC, cx - radius, cy - radius, cx + radius, cy + radius);
        
        SelectObject(m_memDC, oldBrush);
        SelectObject(m_memDC, oldPen);
        DeleteObject(pen);
    }

    // Circle dengan fill saja
    void drawCircle(int cx, int cy, int radius, COLORREF fillColor) {
        HBRUSH brush = CreateSolidBrush(fillColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(m_memDC, brush);
        HPEN oldPen = (HPEN)SelectObject(m_memDC, GetStockObject(NULL_PEN));
        
        Ellipse(m_memDC, cx - radius, cy - radius, cx + radius, cy + radius);
        
        SelectObject(m_memDC, oldBrush);
        SelectObject(m_memDC, oldPen);
        DeleteObject(brush);
    }

    // Circle dengan fill dan stroke
    void drawCircle(int cx, int cy, int radius, COLORREF fillColor, COLORREF strokeColor, int strokeWidth = 1) {
        HBRUSH brush = CreateSolidBrush(fillColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(m_memDC, brush);
        HPEN pen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
        HPEN oldPen = (HPEN)SelectObject(m_memDC, pen);
        
        Ellipse(m_memDC, cx - radius, cy - radius, cx + radius, cy + radius);
        
        SelectObject(m_memDC, oldBrush);
        SelectObject(m_memDC, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
    }

private:
    HWND m_hwnd;
    HDC m_hdc;
    HDC m_memDC;
    HBITMAP m_memBitmap;
    HBITMAP m_oldBitmap;

    void setupDoubleBuffering() {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        
        m_memDC = CreateCompatibleDC(m_hdc);
        m_memBitmap = CreateCompatibleBitmap(m_hdc, rect.right, rect.bottom);
        m_oldBitmap = (HBITMAP)SelectObject(m_memDC, m_memBitmap);
        
        // Clear background
        HBRUSH bg = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(m_memDC, &rect, bg);
        DeleteObject(bg);
    }

    void cleanup() {
        if (m_memDC) {
            if (m_oldBitmap) {
                SelectObject(m_memDC, m_oldBitmap);
                m_oldBitmap = nullptr;
            }
            DeleteDC(m_memDC);
            m_memDC = nullptr;
        }
        if (m_memBitmap) {
            DeleteObject(m_memBitmap);
            m_memBitmap = nullptr;
        }
    }

    void drawRectInternal(z::Bound<int> bound, COLORREF fillColor, COLORREF strokeColor, bool hasFill, bool hasStroke, int strokeWidth) {
        HBRUSH brush = nullptr;
        HPEN pen = nullptr;
        HBRUSH oldBrush = nullptr;
        HPEN oldPen = nullptr;

        if (hasFill) {
            brush = CreateSolidBrush(fillColor);
            oldBrush = (HBRUSH)SelectObject(m_memDC, brush);
        } else
            oldBrush = (HBRUSH)SelectObject(m_memDC, GetStockObject(NULL_BRUSH));

        if (hasStroke) {
            pen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
            oldPen = (HPEN)SelectObject(m_memDC, pen);
        } else 
            oldPen = (HPEN)SelectObject(m_memDC, GetStockObject(NULL_PEN));

        Rectangle(m_memDC, bound.x, bound.y, bound.x + bound.w, bound.y + bound.h);

        SelectObject(m_memDC, oldBrush);
        SelectObject(m_memDC, oldPen);
        
        if (brush) DeleteObject(brush);
        if (pen) DeleteObject(pen);
    }
};

}