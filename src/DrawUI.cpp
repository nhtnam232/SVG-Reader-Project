#include "DrawUI.h"

void DrawHelpOverlay(Gdiplus::Graphics& g, const RECT& rc)
{
    const int padding = 10;
    const int boxWidth = 220;
    const int boxHeight = 100;

    int x = rc.right - boxWidth - 20;
    int y = rc.bottom - boxHeight - 20;

    // Background (semi-transparent)
    Gdiplus::SolidBrush bgBrush(
        Gdiplus::Color(200, 240, 240, 240)); // alpha = 200

    g.FillRectangle(&bgBrush, x, y, boxWidth, boxHeight);

    // Border
    Gdiplus::Pen borderPen(Gdiplus::Color(255, 100, 100, 100), 1.0f);
    g.DrawRectangle(&borderPen, x, y, boxWidth, boxHeight);

    // Text
    Gdiplus::FontFamily fontFamily(L"Segoe UI");
    Gdiplus::Font font(&fontFamily, 12.0f, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 0, 0, 0));

    Gdiplus::RectF textRect(
        (Gdiplus::REAL)(x + padding),
        (Gdiplus::REAL)(y + padding),
        (Gdiplus::REAL)(boxWidth - 2 * padding),
        (Gdiplus::REAL)(boxHeight - 2 * padding)
    );

    const WCHAR* text =
        L"I / O : Zoom in / out\n"
        L"L / R : Rotate left / right\n"
        L"← ↑ ↓ →: Move\n"
        L"Enter : Reset\n"
        L"H : Hide / Show";

    g.DrawString(text, -1, &font, textRect, nullptr, &textBrush);
}