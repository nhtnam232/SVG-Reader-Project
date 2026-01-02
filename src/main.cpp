// main.cpp
#include "myLib.h"
#include "Parser.h"
#include "Shape.h"
//#include "Vec2.h"
#include "Camera.h"
#include "SVGRenderer.h"

static Camera cam;
Parser parser;
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

    string filePath = "samples/Instagram_logo_2016.svg";
    if (__argc > 1) {
        filePath = __argv[1];
    }

    if (!parser.loadFile(filePath.c_str())) {
        std::string msg = "Cannot open SVG File: " + filePath;
        MessageBoxA(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
        return -1; // Thoát nếu không load được
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
            cam.zoomIn(1.1);
        else
            cam.zoomOut(1.1);

        InvalidateRect(hWnd, NULL, TRUE);
    } break;

    case WM_KEYDOWN: {
        // Lấy vị trí chuột để zoom quanh con trỏ
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hWnd, &pt);
        Camera::Vec2 cursorPt = { (double)pt.x, (double)pt.y };

        switch (wParam) {

            // ===== ZOOM =====
        case 'I':
            cam.zoomIn(1.1);   // zoom quanh con trỏ
            break;

        case 'O':
            cam.zoomOut(1.1);  // zoom quanh con trỏ
            break;

            // ===== ROTATE =====
        case 'L':
            cam.rotate(-5.0);
            break;

        case 'R':
            cam.rotate(5.0);
            break;

            // ===== RESET =====
        case '\n':   // Enter
            cam.reset();
            break;
                // ===== PAN BẰNG PHÍM MŨI TÊN (PHẦN THÊM) =====
        case VK_UP:
        case VK_DOWN:
        case VK_LEFT:
        case VK_RIGHT:
        {
            Camera::Vec2 c = cam.getCenter();
            double step = 30.0 / cam.getScale(); // pan mượt theo zoom

            if (wParam == VK_UP)        c.y -= step;
            if (wParam == VK_DOWN)      c.y += step;
            if (wParam == VK_LEFT)      c.x -= step;
            if (wParam == VK_RIGHT)     c.x += step;

            cam.setCenter(c);
            break;
        }

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


    ViewBox vb = parser.getViewBox();
    Gdiplus::Matrix* matrix = vb.getTransformMatrix((float)(rc.right - rc.left), (float)(rc.bottom - rc.top));
    if (matrix) {
        graphics.SetTransform(matrix);

        delete matrix;
    }


    // Apply camera transform via svgRenderer
    svgRenderer.applyTransform(cam, graphics);

    // Render shapes (now render takes Graphics&)
    const std::vector<myShape*>& shapes = parser.getShape();
    svgRenderer.render(shapes, graphics);

    // nếu cần vẽ UI overlay:
    // graphics.ResetTransform();
    // draw UI here
}

