#include "myLib.h"
#include "Parser.h"
#include "Shape.h"

//forward declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID OnPaint(HDC hdc); 


Parser parser;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    
    // Initialize GDI+.
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Input your address SVG file
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
        TEXT("SVGReaderWindowClass"), //window class name
        TEXT("SVG Renderer"),   //window caption
        WS_OVERLAPPEDWINDOW,    //window style
        CW_USEDEFAULT,          //initial x position
        CW_USEDEFAULT,          //initial y position
        1920, 1080,             //initial x, y size     
        NULL, 
        NULL, 
        hInstance, 
        NULL);


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
    switch (message)
    {
    case WM_PAINT: 
    {
        HDC hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps); 
        OnPaint(hdc);                
        EndPaint(hWnd, &ps);         
    }
    return 0;

    case WM_DESTROY: 
    {
        PostQuitMessage(0); 
    }
    return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

VOID OnPaint(HDC hdc)
{

    Gdiplus::Graphics graphics(hdc);

    // Implement High quality
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    Gdiplus::SolidBrush backgroundBrush(Gdiplus::Color(255, 255, 255));
    graphics.FillRectangle(&backgroundBrush, 0, 0, 2000, 2000); 

    const vector<myShape*>& shapes = parser.getShape();


    for (myShape* shape : shapes)
    {
        if (shape != nullptr) {
            shape->draw(graphics);
        }
    }
}