// main.cpp
#include "myLib.h"
#include "Parser.h"
#include "Shape.h"
//#include "Vec2.h"
#include "Camera.h"
#include "SVGRenderer.h"

static Camera cam;
static Parser parser;
static SVGRenderer svgRenderer;

// Forward
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID OnPaint(HDC hdc);


// =========================
// WinMain
// =========================
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    if (!parser.loadFile("samples\\sample.svg")) {
        MessageBox(NULL, TEXT("Cannot open SVG File!"), TEXT("Error"), MB_OK | MB_ICONERROR);
    }

    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = TEXT("SVGReaderWindowClass");
    RegisterClass(&wc);

    HWND hWnd = CreateWindow(
        TEXT("SVGReaderWindowClass"),
        TEXT("SVG Renderer"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);
    return (INT)msg.wParam;
}



// =========================
// Window Proc
// =========================
// Hàm helper lấy tọa độ con trỏ trong client area
Camera::Vec2 getMousePosInClient(HWND hWnd) {
    POINT pt;
    GetCursorPos(&pt);          // lấy vị trí chuột toàn màn hình
    ScreenToClient(hWnd, &pt);  // chuyển sang tọa độ client
    return { (double)pt.x, (double)pt.y };
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    constexpr double MIN_SCALE = 0.1;
    constexpr double MAX_SCALE = 10.0;
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
        EndPaint(hWnd, &ps);
    }
    return 0;

    
    case WM_MOUSEWHEEL: {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        Camera::Vec2 cursorPt;
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        ScreenToClient(hWnd, &pt);
        cursorPt = { (double)pt.x, (double)pt.y };

        if (zDelta > 0)
            cam.zoomIn(1.1, cursorPt);
        else
            cam.zoomOut(1.1, cursorPt);

        InvalidateRect(hWnd, NULL, TRUE);
    } break;

    case WM_KEYDOWN: {
        // Lấy vị trí chuột để zoom quanh con trỏ
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hWnd, &pt);
        Camera::Vec2 cursorPt = { (double)pt.x, (double)pt.y };

        switch (wParam) {
        case VK_UP:
            cam.zoomIn(1.1, cursorPt);   // zoom quanh con trỏ
            break;
        case VK_DOWN:
            cam.zoomOut(1.1, cursorPt);  // zoom quanh con trỏ
            break;
        case VK_LEFT:
            cam.rotate(-5.0);            // rotate quanh center
            break;
        case VK_RIGHT:
            cam.rotate(5.0);             // rotate quanh center
            break;
        case 'R':
            cam.reset();
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
    } break;


    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}



// =========================
// PAINT
// =========================
VOID OnPaint(HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    HWND hWnd = WindowFromDC(hdc);
    RECT rc;
    if (hWnd) GetClientRect(hWnd, &rc);
    else { rc.left = 0; rc.top = 0; rc.right = 1280; rc.bottom = 720; }

    Gdiplus::SolidBrush backgroundBrush(Gdiplus::Color(255, 255, 255, 255));
    graphics.FillRectangle(&backgroundBrush, 0, 0, rc.right - rc.left, rc.bottom - rc.top);

    // Apply camera transform via svgRenderer
    svgRenderer.applyTransform(cam, graphics);

    // Render shapes (now render takes Graphics&)
    const std::vector<myShape*>& shapes = parser.getShape();
    svgRenderer.render(shapes, graphics);

    // nếu cần vẽ UI overlay:
    // graphics.ResetTransform();
    // draw UI here
}

