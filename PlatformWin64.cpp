#include "Fierce3D.h"
#include <Windows.h>
#include "PlatformWin64.h"
#include <iostream>
#include <vector>


#define MAX_LOADSTRING 100
HINSTANCE hInstance;
HWND hWnd;

//HINSTANCE                          hInstance;                 // current instance
std::vector<wchar_t>               szTitle;                   // The title bar text
std::vector<wchar_t>               szWindowClass;             // the main window class name
BOOL                InitInstance(HINSTANCE, int);
int                 WinX, WinY, WinXS, WinYS; 
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


HWND GetWindowHandle() {
    return hWnd;
}

HINSTANCE GetInstance() {
    return hInstance;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {}

// Convert std::string to LPCWSTR - so we can use generic std::string where ever we go!
// Windows wants LPCWSTR for varous reasons, including Window init
LPCWSTR StringToLPCWSTR(const std::string& str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::vector<wchar_t> buffer(size);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer.data(), size);
    return buffer.data();
}


// Convert std::string to wide character array
std::vector<wchar_t> StringToWCharArray(const std::string& str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    // Allocate a vector for the wide character array
    std::vector<wchar_t> wideString(size);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wideString.data(), size);
    // Resize to exclude the null terminator
    wideString.resize(size - 1);
    return wideString;
}


bool CreateWin64Window(Fierce3D::CF3DApp* app, const char* appName, const char* winName, const Fierce3D::SVideoCreateStruct& VCStruct, bool show)
{
    if ( VCStruct.hInstance != nullptr )
        hInstance = VCStruct.hInstance;
    else
        hInstance = GetModuleHandle( 0 );
    if ( VCStruct.hWnd == nullptr )
    {
        szTitle = StringToWCharArray(appName);
        szWindowClass = StringToWCharArray(appName);
        WinX = 0;
        WinY = 0;
        WinXS = VCStruct.width;
        WinYS = VCStruct.height;
        WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FIERCE3D));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FIERCE3D);
        wcex.lpszClassName = szWindowClass.data();
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
        RegisterClassExW(&wcex);
        hWnd = CreateWindowW(szWindowClass.data(),
            szTitle.data(),
            WS_OVERLAPPEDWINDOW,
            WinX,
            WinY,
            WinXS,
            WinYS,
            0,
            nullptr,
            hInstance,
            nullptr);
        if (!hWnd) {
            return FALSE;
        }

    }
    else
        hWnd = VCStruct.hWnd;
    ShowWindow(hWnd, show );
    UpdateWindow(hWnd);
    // Set up App Function Pointers
//    typedef int* (*F3DFunctionPointer)(const int F3DEVT_TYPE, const void* Params, const int paramCnt);
    return true;
}



WPARAM WinMsgLoop()
{
    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_FIERCE3D ) );
    MSG msg;
    while ( GetMessage(&msg, nullptr, 0, 0 ) ) {
        if ( !TranslateAccelerator(msg.hwnd, hAccelTable, &msg ) ) {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }
    return msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch ( message ) {
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId) {
        case IDM_ABOUT:
            DialogBox( hInstance, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, About );
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

