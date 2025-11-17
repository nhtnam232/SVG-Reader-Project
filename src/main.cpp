// fixed_svg_viewer.cpp
#include "myLib.h"
#include "Parser.h"
#include "Shape.h"

#ifndef HAS_STD_CLAMP
template <typename T>
inline const T& clamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : ((v > hi) ? hi : v);
}
#endif

struct Vec2 { double x; double y; };
static const double PI = 3.14159265358979323846;

static Vec2 make_vec(LONG x, LONG y) { return { (double)x, (double)y }; }
static Vec2 rotate(const Vec2& v, double angleDeg) {
    double a = angleDeg * PI / 180.0;
    double c = std::cos(a), s = std::sin(a);
    return { c * v.x - s * v.y, s * v.x + c * v.y };
}
static Vec2 rotate_inv(const Vec2& v, double angleDeg) {
    return rotate(v, -angleDeg);
}

struct Camera {
    double scale = 1.0;
    double angle = 0.0; // degrees
    POINT center = { 960, 540 };
};

static Camera cam;

static Vec2 screenToWorld(const Vec2& screenPt, const Camera& c) {
    Vec2 center = { (double)c.center.x, (double)c.center.y };
    Vec2 diff = { screenPt.x - center.x, screenPt.y - center.y };
    Vec2 unrot = rotate_inv(diff, c.angle);
    return { center.x + unrot.x / c.scale, center.y + unrot.y / c.scale };
}

static Vec2 computeCenterForZoom(const Vec2& p, const Vec2& w, double sNew, double angleDeg) {
    double a = angleDeg * PI / 180.0;
    double ca = std::cos(a), sa = std::sin(a);

    double a11 = 1.0 - sNew * ca;
    double a12 = sNew * sa;
    double a21 = -sNew * sa;
    double a22 = 1.0 - sNew * ca;

    double det = a11 * a22 - a12 * a21;
    const double EPS = 1e-9;
    if (std::abs(det) < EPS) {
        // fallback: giữ center hiện tại
        return { (double)cam.center.x, (double)cam.center.y };
    }

    double Awx = sNew * (ca * w.x - sa * w.y);
    double Awy = sNew * (sa * w.x + ca * w.y);
    double rhsx = p.x - Awx;
    double rhsy = p.y - Awy;

    double inv11 = a22 / det;
    double inv12 = -a12 / det;
    double inv21 = -a21 / det;
    double inv22 = a11 / det;

    double cx = inv11 * rhsx + inv12 * rhsy;
    double cy = inv21 * rhsx + inv22 * rhsy;
    return { cx, cy };
}

Parser parser;

// Forward
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID OnPaint(HDC hdc);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    if (!parser.loadFile("samples\\sample.svg")) {
        MessageBox(NULL, TEXT("Cannot open SVG File!"), TEXT("Error"), MB_OK | MB_ICONERROR);
    }

    WNDCLASS wndClass = {};
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszClassName = TEXT("SVGReaderWindowClass");
    RegisterClass(&wndClass);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    const double MIN_SCALE = 0.1;
    const double MAX_SCALE = 10.0;

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

    case WM_MOUSEWHEEL:
    {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        double factor = (zDelta > 0) ? 1.1 : 0.9;
        double oldScale = cam.scale;
        double newScale = clamp(oldScale * factor, MIN_SCALE, MAX_SCALE);

        if (std::abs(newScale - oldScale) < 1e-12) break;

        POINT pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        ScreenToClient(hWnd, &pt);
        Vec2 p = make_vec(pt.x, pt.y);

        Vec2 w = screenToWorld(p, cam);
        Vec2 newCenter = computeCenterForZoom(p, w, newScale, cam.angle);

        cam.scale = newScale;
        cam.center.x = (LONG)std::lround(newCenter.x);
        cam.center.y = (LONG)std::lround(newCenter.y);

        InvalidateRect(hWnd, NULL, TRUE);
    }
    return 0;

    case WM_KEYDOWN:
    {
        switch (wParam) {
        case VK_UP:    cam.scale = clamp(cam.scale * 1.1, MIN_SCALE, MAX_SCALE); break;
        case VK_DOWN:  cam.scale = clamp(cam.scale * 0.9, MIN_SCALE, MAX_SCALE); break;
        case VK_LEFT:  cam.angle -= 5.0; break;
        case VK_RIGHT: cam.angle += 5.0; break;
        case 'R':      cam.scale = 1.0; cam.angle = 0.0; cam.center = { 960, 540 }; break;
        default: break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
    }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

VOID OnPaint(HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    Gdiplus::SolidBrush backgroundBrush(Gdiplus::Color(255, 255, 255, 255));

    // Lấy HWND từ HDC (chính xác hơn so với GetActiveWindow)
    HWND hWnd = WindowFromDC(hdc);
    RECT rc;
    if (hWnd != NULL) {
        GetClientRect(hWnd, &rc);
    }
    else {
        rc.left = 0; rc.top = 0; rc.right = 1280; rc.bottom = 720;
    }
    graphics.FillRectangle(&backgroundBrush, 0, 0, rc.right - rc.left, rc.bottom - rc.top);

    // CAMERA TRANSFORM
    graphics.TranslateTransform((Gdiplus::REAL)cam.center.x, (Gdiplus::REAL)cam.center.y);
    graphics.RotateTransform((Gdiplus::REAL)cam.angle);
    graphics.ScaleTransform((Gdiplus::REAL)cam.scale, (Gdiplus::REAL)cam.scale);
    graphics.TranslateTransform(-(Gdiplus::REAL)cam.center.x, -(Gdiplus::REAL)cam.center.y);

    const std::vector<myShape*>& shapes = parser.getShape();
    for (myShape* shape : shapes)
    {
        if (shape) shape->draw(graphics);
    }
}

// Optional: reset transform if needed
// graphics.ResetTransform();

//Zoom bằng con lăn chuột: đặt con trỏ chuột lên chỗ muốn phóng to/thu nhỏ, cuộn lên để zoom in (tăng scale), cuộn xuống để zoom out. Zoom sẽ cố giữ điểm thế giới dưới con trỏ không đổi (nếu phép toán computeCenterForZoom không bị suy biến).
//
//Zoom bằng bàn phím: phím ↑ (Up) zoom in (tăng 10%), phím ↓ (Down) zoom out (giảm 10%). (Cửa sổ phải có focus để nhận phím.)
//
//Rotate bằng bàn phím: phím ← (Left) quay trái 5°, phím → (Right) quay phải 5°. Quay quanh cam.center.
//
//Reset camera: phím R đặt scale = 1.0, angle = 0.0, center = {960,540}

