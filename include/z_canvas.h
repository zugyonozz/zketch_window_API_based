#pragma once
#include <windows.h>
#include <memory>
#include "z_unit.h"

namespace z {

class Canvas {
public:
    // Constructor
    Canvas(HWND hwnd) : m_hwnd(hwnd), m_hdc(nullptr), m_memDC(nullptr), m_memBitmap(nullptr), m_oldBitmap(nullptr) {
        m_hdc = GetDC(hwnd);
        setupDoubleBuffering();
    }

    // Destructor
    ~Canvas() {
        cleanup();
        if (m_hdc)
            ReleaseDC(m_hwnd, m_hdc);
    }

    // Disable copy constructor dan assignment operator
    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    // ===== BASIC OPERATIONS =====
    
    // Clear canvas dengan warna tertentu
    void clear(COLORREF color = RGB(0, 0, 0)) {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        HBRUSH bg = CreateSolidBrush(color);
        FillRect(m_memDC, &rect, bg);
        DeleteObject(bg);
    }

    // Clear dengan Color struct
    void clear(Color<unsigned char> color) {
        clear(RGB(color.r, color.g, color.b));
    }

    // Present buffer ke layar
    void present() {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        BitBlt(m_hdc, 0, 0, rect.right, rect.bottom, m_memDC, 0, 0, SRCCOPY);
    }

    // Resize canvas (dipanggil saat window resize)
    void resize() {
        cleanup();
        setupDoubleBuffering();
    }

    // Get HDC untuk operasi advanced
    HDC getHDC() const { return m_memDC; }
    HWND getHWND() const { return m_hwnd; }

    // ===== BASIC DRAWING =====

    // Draw pixel
    void drawPixel(int x, int y, COLORREF color = RGB(255, 255, 255)) {
        SetPixel(m_memDC, x, y, color);
    }

    void drawPixel(Vec2<int> pos, COLORREF color = RGB(255, 255, 255)) {
        SetPixel(m_memDC, pos.x, pos.y, color);
    }

    void drawPixel(Vec2<int> pos, Color<unsigned char> color) {
        SetPixel(m_memDC, pos.x, pos.y, RGB(color.r, color.g, color.b));
    }

    // Draw line
    void drawLine(int x1, int y1, int x2, int y2, COLORREF color = RGB(255, 255, 255), int width = 1) {
        HPEN pen = CreatePen(PS_SOLID, width, color);
        HPEN oldPen = (HPEN)SelectObject(m_memDC, pen);
        
        MoveToEx(m_memDC, x1, y1, nullptr);
        LineTo(m_memDC, x2, y2);
        
        SelectObject(m_memDC, oldPen);
        DeleteObject(pen);
    }

    void drawLine(Vec2<int> start, Vec2<int> end, COLORREF color = RGB(255, 255, 255), int width = 1) {
        drawLine(start.x, start.y, end.x, end.y, color, width);
    }

    void drawLine(Vec2<int> start, Vec2<int> end, Color<unsigned char> color, int width = 1) {
        drawLine(start.x, start.y, end.x, end.y, RGB(color.r, color.g, color.b), width);
    }

    // ===== RECTANGLE DRAWING =====

    // Basic rectangle - outline only
    void drawRect(int x, int y, int width, int height) {
        drawRectInternal(x, y, width, height, RGB(0, 0, 0), RGB(255, 255, 255), false, true, 1);
    }

    void drawRect(Vec2<int> position, Vec2<int> size) {
        drawRect(position.x, position.y, size.x, size.y);
    }

    void drawRect(Rect<int> rect) {
        drawRect(rect.x, rect.y, rect.w, rect.h);
    }

    // Rectangle with stroke color
    void drawRect(int x, int y, int width, int height, COLORREF strokeColor, int strokeWidth = 1) {
        drawRectInternal(x, y, width, height, RGB(0, 0, 0), strokeColor, false, true, strokeWidth);
    }

    void drawRect(Vec2<int> position, Vec2<int> size, COLORREF strokeColor, int strokeWidth = 1) {
        drawRect(position.x, position.y, size.x, size.y, strokeColor, strokeWidth);
    }

    void drawRect(Rect<int> rect, COLORREF strokeColor, int strokeWidth = 1) {
        drawRect(rect.x, rect.y, rect.w, rect.h, strokeColor, strokeWidth);
    }

    void drawRect(Rect<int> rect, Color<unsigned char> strokeColor, int strokeWidth = 1) {
        drawRect(rect.x, rect.y, rect.w, rect.h, RGB(strokeColor.r, strokeColor.g, strokeColor.b), strokeWidth);
    }

    // ===== FILLED RECTANGLE =====

    // Filled rectangle
    void fillRect(int x, int y, int width, int height, COLORREF fillColor = RGB(255, 255, 255)) {
        drawRectInternal(x, y, width, height, fillColor, RGB(0, 0, 0), true, false, 1);
    }

    void fillRect(Vec2<int> position, Vec2<int> size, COLORREF fillColor = RGB(255, 255, 255)) {
        fillRect(position.x, position.y, size.x, size.y, fillColor);
    }

    void fillRect(Rect<int> rect, COLORREF fillColor = RGB(255, 255, 255)) {
        fillRect(rect.x, rect.y, rect.w, rect.h, fillColor);
    }

    void fillRect(Rect<int> rect, Color<unsigned char> fillColor) {
        fillRect(rect.x, rect.y, rect.w, rect.h, RGB(fillColor.r, fillColor.g, fillColor.b));
    }

    // Filled rectangle with stroke
    void fillRect(int x, int y, int width, int height, COLORREF fillColor, COLORREF strokeColor, int strokeWidth = 1) {
        drawRectInternal(x, y, width, height, fillColor, strokeColor, true, true, strokeWidth);
    }

    void fillRect(Vec2<int> position, Vec2<int> size, COLORREF fillColor, COLORREF strokeColor, int strokeWidth = 1) {
        fillRect(position.x, position.y, size.x, size.y, fillColor, strokeColor, strokeWidth);
    }

    void fillRect(Rect<int> rect, COLORREF fillColor, COLORREF strokeColor, int strokeWidth = 1) {
        fillRect(rect.x, rect.y, rect.w, rect.h, fillColor, strokeColor, strokeWidth);
    }

    void fillRect(Rect<int> rect, Color<unsigned char> fillColor, Color<unsigned char> strokeColor, int strokeWidth = 1) {
        fillRect(rect.x, rect.y, rect.w, rect.h, RGB(fillColor.r, fillColor.g, fillColor.b), RGB(strokeColor.r, strokeColor.g, strokeColor.b), strokeWidth);
    }

    // ===== CIRCLE DRAWING =====

    // Circle outline only
    void drawCircle(int centerX, int centerY, int radius, COLORREF strokeColor = RGB(255, 255, 255), int strokeWidth = 1) {
        HPEN pen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
        HPEN oldPen = (HPEN)SelectObject(m_memDC, pen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(m_memDC, GetStockObject(NULL_BRUSH));
        
        Ellipse(m_memDC, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
        
        SelectObject(m_memDC, oldBrush);
        SelectObject(m_memDC, oldPen);
        DeleteObject(pen);
    }

    void drawCircle(Vec2<int> center, int radius, COLORREF strokeColor = RGB(255, 255, 255), int strokeWidth = 1) {
        drawCircle(center.x, center.y, radius, strokeColor, strokeWidth);
    }

    void drawCircle(Vec2<int> center, int radius, Color<unsigned char> strokeColor, int strokeWidth = 1) {
        drawCircle(center.x, center.y, radius, RGB(strokeColor.r, strokeColor.g, strokeColor.b), strokeWidth);
    }

    // ===== FILLED CIRCLE =====

    // Filled circle
    void fillCircle(int centerX, int centerY, int radius, COLORREF fillColor = RGB(255, 255, 255)) {
        HBRUSH brush = CreateSolidBrush(fillColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(m_memDC, brush);
        HPEN oldPen = (HPEN)SelectObject(m_memDC, GetStockObject(NULL_PEN));
        
        Ellipse(m_memDC, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
        
        SelectObject(m_memDC, oldBrush);
        SelectObject(m_memDC, oldPen);
        DeleteObject(brush);
    }

    void fillCircle(Vec2<int> center, int radius, COLORREF fillColor = RGB(255, 255, 255)) {
        fillCircle(center.x, center.y, radius, fillColor);
    }

    void fillCircle(Vec2<int> center, int radius, Color<unsigned char> fillColor) {
        fillCircle(center.x, center.y, radius, RGB(fillColor.r, fillColor.g, fillColor.b));
    }

    // Filled circle with stroke
    void fillCircle(int centerX, int centerY, int radius, COLORREF fillColor, COLORREF strokeColor, int strokeWidth = 1) {
        HBRUSH brush = CreateSolidBrush(fillColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(m_memDC, brush);
        HPEN pen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
        HPEN oldPen = (HPEN)SelectObject(m_memDC, pen);
        
        Ellipse(m_memDC, centerX - radius, centerY - radius, centerX + radius, centerX + radius);
        
        SelectObject(m_memDC, oldBrush);
        SelectObject(m_memDC, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
    }

    void fillCircle(Vec2<int> center, int radius, COLORREF fillColor, COLORREF strokeColor, int strokeWidth = 1) {
        fillCircle(center.x, center.y, radius, fillColor, strokeColor, strokeWidth);
    }

    void fillCircle(Vec2<int> center, int radius, Color<unsigned char> fillColor, Color<unsigned char> strokeColor, int strokeWidth = 1) {
        fillCircle(center.x, center.y, radius, RGB(fillColor.r, fillColor.g, fillColor.b), RGB(strokeColor.r, strokeColor.g, strokeColor.b), strokeWidth);
    }

    // ===== ELLIPSE DRAWING =====

    // Ellipse outline only
    void drawEllipse(int centerX, int centerY, int radiusX, int radiusY, COLORREF strokeColor = RGB(255, 255, 255), int strokeWidth = 1) {
        HPEN pen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
        HPEN oldPen = (HPEN)SelectObject(m_memDC, pen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(m_memDC, GetStockObject(NULL_BRUSH));
        
        Ellipse(m_memDC, centerX - radiusX, centerY - radiusY, centerX + radiusX, centerY + radiusY);
        
        SelectObject(m_memDC, oldBrush);
        SelectObject(m_memDC, oldPen);
        DeleteObject(pen);
    }

    void drawEllipse(Vec2<int> center, Vec2<int> radius, COLORREF strokeColor = RGB(255, 255, 255), int strokeWidth = 1) {
        drawEllipse(center.x, center.y, radius.x, radius.y, strokeColor, strokeWidth);
    }

    void drawEllipse(Rect<int> bounds, COLORREF strokeColor = RGB(255, 255, 255), int strokeWidth = 1) {
        drawEllipse(bounds.x + bounds.w/2, bounds.y + bounds.h/2, bounds.w/2, bounds.h/2, strokeColor, strokeWidth);
    }

    // Filled ellipse
    void fillEllipse(int centerX, int centerY, int radiusX, int radiusY, COLORREF fillColor = RGB(255, 255, 255)) {
        HBRUSH brush = CreateSolidBrush(fillColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(m_memDC, brush);
        HPEN oldPen = (HPEN)SelectObject(m_memDC, GetStockObject(NULL_PEN));
        
        Ellipse(m_memDC, centerX - radiusX, centerY - radiusY, centerX + radiusX, centerY + radiusY);
        
        SelectObject(m_memDC, oldBrush);
        SelectObject(m_memDC, oldPen);
        DeleteObject(brush);
    }

    void fillEllipse(Vec2<int> center, Vec2<int> radius, COLORREF fillColor = RGB(255, 255, 255)) {
        fillEllipse(center.x, center.y, radius.x, radius.y, fillColor);
    }

    void fillEllipse(Rect<int> bounds, COLORREF fillColor = RGB(255, 255, 255)) {
        fillEllipse(bounds.x + bounds.w/2, bounds.y + bounds.h/2, bounds.w/2, bounds.h/2, fillColor);
    }

    // ===== POLYGON DRAWING =====

    // Draw polygon (outline only)
    void drawPolygon(const POINT* points, int count, COLORREF strokeColor = RGB(255, 255, 255), int strokeWidth = 1) {
        HPEN pen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
        HPEN oldPen = (HPEN)SelectObject(m_memDC, pen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(m_memDC, GetStockObject(NULL_BRUSH));
        
        Polygon(m_memDC, points, count);
        
        SelectObject(m_memDC, oldBrush);
        SelectObject(m_memDC, oldPen);
        DeleteObject(pen);
    }

    void drawPolygon(const Vec2<int>* points, int count, COLORREF strokeColor = RGB(255, 255, 255), int strokeWidth = 1) {
        POINT* winPoints = new POINT[count];
        for (int i = 0; i < count; i++) {
            winPoints[i].x = points[i].x;
            winPoints[i].y = points[i].y;
        }
        drawPolygon(winPoints, count, strokeColor, strokeWidth);
        delete[] winPoints;
    }

    // Fill polygon
    void fillPolygon(const POINT* points, int count, COLORREF fillColor = RGB(255, 255, 255)) {
        HBRUSH brush = CreateSolidBrush(fillColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(m_memDC, brush);
        HPEN oldPen = (HPEN)SelectObject(m_memDC, GetStockObject(NULL_PEN));
        
        Polygon(m_memDC, points, count);
        
        SelectObject(m_memDC, oldBrush);
        SelectObject(m_memDC, oldPen);
        DeleteObject(brush);
    }

    void fillPolygon(const Vec2<int>* points, int count, COLORREF fillColor = RGB(255, 255, 255)) {
        POINT* winPoints = new POINT[count];
        for (int i = 0; i < count; i++) {
            winPoints[i].x = points[i].x;
            winPoints[i].y = points[i].y;
        }
        fillPolygon(winPoints, count, fillColor);
        delete[] winPoints;
    }

    // ===== UTILITY FUNCTIONS =====

    // Convert Color to COLORREF
    static COLORREF toColorRef(Color<unsigned char> color) {
        return RGB(color.r, color.g, color.b);
    }

    // Convert COLORREF to Color
    static Color<unsigned char> fromColorRef(COLORREF color) {
        return Color<unsigned char>(GetRValue(color), GetGValue(color), GetBValue(color), 255);
    }

    // Get canvas size
    Vec2<int> getSize() const {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        return Vec2<int>(rect.right - rect.left, rect.bottom - rect.top);
    }

    // Get canvas bounds
    Rect<int> getBounds() const {
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        return Rect<int>(0, 0, rect.right - rect.left, rect.bottom - rect.top);
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

    void drawRectInternal(int x, int y, int width, int height, COLORREF fillColor, COLORREF strokeColor, bool hasFill, bool hasStroke, int strokeWidth) {
        HBRUSH brush = nullptr;
        HPEN pen = nullptr;
        HBRUSH oldBrush = nullptr;
        HPEN oldPen = nullptr;

        if (hasFill) {
            brush = CreateSolidBrush(fillColor);
            oldBrush = (HBRUSH)SelectObject(m_memDC, brush);
        } else {
            oldBrush = (HBRUSH)SelectObject(m_memDC, GetStockObject(NULL_BRUSH));
        }

        if (hasStroke) {
            pen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
            oldPen = (HPEN)SelectObject(m_memDC, pen);
        } else {
            oldPen = (HPEN)SelectObject(m_memDC, GetStockObject(NULL_PEN));
        }

        Rectangle(m_memDC, x, y, x + width, y + height);

        SelectObject(m_memDC, oldBrush);
        SelectObject(m_memDC, oldPen);
        
        if (brush) DeleteObject(brush);
        if (pen) DeleteObject(pen);
    }
};

} // namespace z