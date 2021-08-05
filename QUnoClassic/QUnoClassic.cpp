// QUnoBare - QUnoClassic.cpp
// 2021 Roger Deetz

#include "framework.h"
#include "QUnoClassic.h"

#define MAX_LOADSTRING 128

HINSTANCE _hInstance;
WCHAR _szWindowTitle[MAX_LOADSTRING];
WCHAR _szWindowClass[MAX_LOADSTRING];

ATOM RegisterWndClass(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPreviousInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPreviousInstance);
    UNREFERENCED_PARAMETER(pCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, _szWindowTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_QUNOCLASSIC, _szWindowClass, MAX_LOADSTRING);
    RegisterWndClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelerators = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUNOCLASSIC));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelerators, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM RegisterWndClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUNOCLASSIC));
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QUNOCLASSIC);
    wcex.lpszClassName  = _szWindowClass;

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   _hInstance = hInstance;

   HWND hWnd = CreateWindowW(_szWindowClass, _szWindowTitle, WS_OVERLAPPEDWINDOW, 
       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(_hInstance, MAKEINTRESOURCE(IDD_ABOUTDLG), hWnd, AboutDlgProc);
            break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // TODO Draw here.

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

INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        break;
    }

    return (INT_PTR)FALSE;
}
