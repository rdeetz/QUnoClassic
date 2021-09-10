// QUnoClassic - QUnoClassic.cpp
// 2021 Roger Deetz

#include "framework.h"
#include "QUnoClassic.h"
#include "QUnoLib.h"

#define MAX_LOADSTRING 128

HINSTANCE _hInstance;
WCHAR _szWindowTitle[MAX_LOADSTRING];
WCHAR _szWindowClass[MAX_LOADSTRING];

ATOM RegisterWndClass(HINSTANCE);
BOOL InitInstance(HINSTANCE, INT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK OptionsDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

WCHAR _szDefaultPlayerName[MAX_LOADSTRING];
WCHAR _szDefaultComputerPlayers[MAX_LOADSTRING];

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPreviousInstance, _In_ LPWSTR lpCmdLine, _In_ INT nCmdShow)
{
    UNREFERENCED_PARAMETER(hPreviousInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadString(hInstance, IDS_QUNO, _szWindowTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_QUNOCLASSIC, _szWindowClass, MAX_LOADSTRING);
    RegisterWndClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    LoadString(hInstance, IDS_DEFAULTPLAYERNAME, _szDefaultPlayerName, MAX_LOADSTRING);
    LoadString(hInstance, IDS_DEFAULTCOMPUTERPLAYERS, _szDefaultComputerPlayers, MAX_LOADSTRING);

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
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_QUNOCLASSIC_SMALL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QUNOCLASSIC);
    wcex.lpszClassName  = _szWindowClass;

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, INT nCmdShow)
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
        INT wmId = LOWORD(wParam);

        switch (wmId)
        {
        case IDM_NEW:
            // TODO Start a new game.
            //      Create players: 1 human, 3 computer
            //      Create a game.
            //      Add players to game.
            //      Start game.

            break;

        case IDM_OPTIONS:
            DialogBox(_hInstance, MAKEINTRESOURCE(IDD_OPTIONSDLG), hWnd, OptionsDlgProc);
            break;

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

INT_PTR CALLBACK OptionsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        // TODO Set the initial player name to the default.
        //      Set the initial number of computer players to the default.
        SetDlgItemText(hDlg, IDC_DEFAULTPLAYERNAME, _szDefaultPlayerName);
        SetDlgItemText(hDlg, IDC_DEFAULTCOMPUTERPLAYERS, _szDefaultComputerPlayers);

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

/*
TODO May one day want to implement an up-down control.
1. Need to include commctrl.h
2. Need to link with commctl.lib (and manifest?)
3. Get HWND of the edit control IDC_DEFAULTCOMPUTERPLAYERS in WM_INITDIALOG GetDlgItem?
4. Create the up-down control as below, but don't use UDS_AUTOBUDDY
5. Set the buddy window SendMessage(UDM_SETBUDDY, hwndBuddy)
HWND CreateUpDnCtl(HWND hwndParent)
{
    icex.dwICC = ICC_UPDOWN_CLASS;    // Set the Initialization Flag value.
    InitCommonControlsEx(&icex);      // Initialize the Common Controls Library.

    hControl = CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING,
                              UPDOWN_CLASS,
                              NULL,
                              WS_CHILDWINDOW | WS_VISIBLE
                              | UDS_AUTOBUDDY | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_HOTTRACK,
                              0, 0,
                              0, 0,         // Set to zero to automatically size to fit the buddy window.
                              hwndParent,
                              NULL,
                              g_hInst,
                              NULL);

    SendMessage(hControl, UDM_SETRANGE, 0, MAKELPARAM(valMax, valMin));    // Sets the controls direction 
                                                                           // and range.

    return (hControl);
}
*/

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
