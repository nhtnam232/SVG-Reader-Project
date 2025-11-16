// --- main.cpp ---

// BƯỚC 1: BAO GỒM CÁC THƯ VIỆN CẦN THIẾT
// "myLib.h" nên chứa <windows.h>, <gdiplus.h> và #pragma comment
#include "myLib.h"
#include "Parser.h"
#include "Shape.h"

// BƯỚC 2: KHAI BÁO CÁC HÀM
// Khai báo trước (forward declaration) cho các hàm xử lý của Windows
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID OnPaint(HDC hdc); // Hàm riêng của chúng ta để vẽ

// BƯỚC 3: KHAI BÁO BIẾN TOÀN CỤC
// Để đơn giản, chúng ta dùng một đối tượng parser toàn cục
// (Trong dự án lớn, người ta sẽ dùng cách khác để truyền dữ liệu)
Parser g_parser;

// BƯỚC 4: HÀM WINMAIN (ĐIỂM VÀO CHƯƠNG TRÌNH)
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    // --- KHỞI TẠO GDI+ ---
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // --- TẢI FILE SVG (Tích hợp dự án của bạn) ---
    if (!g_parser.loadFile("sample.svg")) {
        MessageBox(NULL, TEXT("Không thể tải file sample.svg!"), TEXT("Lỗi"), MB_OK | MB_ICONERROR);
    }

    // --- ĐĂNG KÝ LỚP CỬA SỔ (Win32 Boilerplate) ---
    WNDCLASS wndClass = {}; // Khởi tạo tất cả về 0
    wndClass.style = CS_HREDRAW | CS_VREDRAW; // Vẽ lại khi kích thước thay đổi
    wndClass.lpfnWndProc = WndProc;            // Hàm xử lý sự kiện
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszClassName = TEXT("SVGReaderWindowClass");
    RegisterClass(&wndClass);

    // --- TẠO CỬA SỔ (Win32 Boilerplate) ---
    HWND hWnd = CreateWindow(
        TEXT("SVGReaderWindowClass"), // Tên lớp đã đăng ký
        TEXT("GDI+ SVG Renderer"),   // Tiêu đề cửa sổ
        WS_OVERLAPPEDWINDOW,       // Kiểu cửa sổ
        CW_USEDEFAULT, CW_USEDEFAULT, // Vị trí (x, y)
        800, 600,                   // Kích thước (width, height)
        NULL, NULL, hInstance, NULL);

    // --- HIỂN THỊ CỬA SỔ VÀ VÒNG LẶP THÔNG ĐIỆP ---
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    // Vòng lặp thông điệp (Message Loop)
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // --- DỌN DẸP GDI+ TRƯỚC KHI THOÁT ---
    Gdiplus::GdiplusShutdown(gdiplusToken);
    return (INT)msg.wParam;
}

// BƯỚC 5: HÀM XỬ LÝ SỰ KIỆN (WNDPROC)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT: // <-- Quan trọng nhất: Khi Windows yêu cầu vẽ lại
    {
        HDC hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps); // Bắt đầu phiên vẽ
        OnPaint(hdc);                // Gọi hàm vẽ của chúng ta
        EndPaint(hWnd, &ps);         // Kết thúc phiên vẽ
    }
    return 0;

    case WM_DESTROY: // <-- Khi người dùng nhấn nút X (đóng)
    {
        PostQuitMessage(0); // Gửi tín hiệu thoát cho Vòng lặp thông điệp
    }
    return 0;

    default:
        // Các sự kiện khác (như chuột, bàn phím)
        // Hãy để Windows xử lý mặc định
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

// BƯỚC 6: HÀM VẼ (ONPAINT) - NƠI TÍCH HỢP OOP
VOID OnPaint(HDC hdc)
{
    // 1. Tạo đối tượng Gdiplus::Graphics từ HDC của Windows
    Gdiplus::Graphics graphics(hdc);

    // 2. (Nên làm) Cài đặt chất lượng vẽ cao (chống răng cưa)
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    // 3. Xóa nền (vẽ một hình chữ nhật màu trắng lớn)
    Gdiplus::SolidBrush backgroundBrush(Gdiplus::Color(255, 255, 255));
    graphics.FillRectangle(&backgroundBrush, 0, 0, 2000, 2000); // Kích thước đủ lớn

    // 4. Lấy danh sách các hình từ Parser
    const std::vector<myShape*>& shapes = g_parser.getShape();

    // 5. Vòng lặp ĐA HÌNH (Polymorphism)
    // Đây là phần ma thuật của OOP
    for (myShape* shape : shapes)
    {
        if (shape != nullptr) {
            // Gọi hàm draw() của từng hình
            // C++ sẽ tự động gọi hàm (Circle::draw, Rect::draw, v.v.)
            //shape->draw(graphics);
        }
    }
}