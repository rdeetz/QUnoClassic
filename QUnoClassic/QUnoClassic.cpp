// QUnoClassic - QUnoClassic.cpp
// 2021 Roger Deetz

#include "framework.h"
#include "QUnoClassic.h"
#include "QUnoLib.h"

#define MAX_LOADSTRING 128
#define Q_FILEVERSION L"FileVersion"

HINSTANCE _hInstance;
TCHAR _szWindowTitle[MAX_LOADSTRING];
TCHAR _szWindowClass[MAX_LOADSTRING];
HFONT _hBoldFont;

ATOM RegisterWndClass(HINSTANCE);
BOOL InitInstance(HINSTANCE, INT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK OptionsDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL GetFileVersionString(LPTSTR, LPTSTR, LPTSTR, UINT);

TCHAR _szDefaultPlayerName[MAX_LOADSTRING];
UINT _nDefaultComputerPlayers = 3;

struct LANGUAGEANDCODEPAGE
{
    WORD wLanguage;
    WORD wCodePage;
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPreviousInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow)
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

    // TODO If the Registry key and values exist, read in the defaults.
    LoadString(hInstance, IDS_DEFAULTPLAYERNAME, _szDefaultPlayerName, MAX_LOADSTRING);

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

    RECT rcOwner;
    RECT rcDlg;
    RECT rcNew;
    HWND hwndOwner;

    switch (message)
    {
    case WM_INITDIALOG:
        hwndOwner = GetParent(hDlg);
        GetWindowRect(hwndOwner, &rcOwner);
        GetWindowRect(hDlg, &rcDlg);
        
        CopyRect(&rcNew, &rcOwner);
        OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
        OffsetRect(&rcNew, -rcNew.left, -rcNew.top);
        OffsetRect(&rcNew, -rcDlg.right, -rcDlg.bottom);

        SetWindowPos(hDlg, HWND_TOP, rcOwner.left + (rcNew.right / 2), rcOwner.top + (rcNew.bottom / 2), 0, 0, SWP_NOSIZE);

        // TODO Create an up-down control rather than a plain edit control 
        //      for the default number of computer players.
        //      https://docs.microsoft.com/en-us/windows/win32/controls/create-an-up-down-control
        //
        //      1. Include commctrl.h
        //      2. Link with commctl.lib (and specify the manifest?)
        //      3. Get HWND of the edit control IDC_DEFAULTCOMPUTERPLAYERS in WM_INITDIALOG via GetDlgItem?
        //      4. Create the up-down control as in the example, but don't set UDS_AUTOBUDDY
        //      5. Set the buddy window via SendMessage(UDM_SETBUDDY, hwndEdit)

        SetDlgItemText(hDlg, IDC_DEFAULTPLAYERNAME, _szDefaultPlayerName);
        SetDlgItemInt(hDlg, IDC_DEFAULTCOMPUTERPLAYERS, _nDefaultComputerPlayers, FALSE);

        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            // TODO Validate the values in the edit controls:
            //      Not null or empty
            //      For name, trim it
            //      For name, not longer than MAX_LOADSTRING
            //      For number, must be parsable as unsigned int
            //      For number, greater than 0 and less than 9
            GetDlgItemText(hDlg, IDC_DEFAULTPLAYERNAME, _szDefaultPlayerName, MAX_LOADSTRING);
            BOOL bTranslated;
            UINT nResult = GetDlgItemInt(hDlg, IDC_DEFAULTCOMPUTERPLAYERS, &bTranslated, FALSE);

            if (bTranslated == TRUE)
            {
                _nDefaultComputerPlayers = nResult;
            }

            // TODO If validations fail, show a message box.
            //      If validations succeed, copy the values to the global defaults.
            //      Save te values to the Registry (create the key if it doesn't exist).
           
            EndDialog(hDlg, LOWORD(wParam));

            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        break;
    }

    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    RECT rcOwner;
    RECT rcDlg;
    RECT rcNew;
    HWND hwndOwner;
    HWND hwndProduct;
    HFONT hOldFont;
    LOGFONT lf;
    BOOL bResult;

    switch (message)
    {
    case WM_INITDIALOG:
        hwndOwner = GetParent(hDlg);
        GetWindowRect(hwndOwner, &rcOwner);
        GetWindowRect(hDlg, &rcDlg);
        
        CopyRect(&rcNew, &rcOwner);
        OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
        OffsetRect(&rcNew, -rcNew.left, -rcNew.top);
        OffsetRect(&rcNew, -rcDlg.right, -rcDlg.bottom);

        SetWindowPos(hDlg, HWND_TOP, rcOwner.left + (rcNew.right / 2), rcOwner.top + (rcNew.bottom / 2), 0, 0, SWP_NOSIZE);

        hwndProduct = GetDlgItem(hDlg, IDC_QUNO_PRODUCT);
        hOldFont = (HFONT)SendMessage(hwndProduct, WM_GETFONT, 0, 0);
        GetObject(hOldFont, sizeof(LOGFONT), (LPVOID)&lf);
        lf.lfWeight = FW_BOLD;
        _hBoldFont = CreateFontIndirect(&lf);
        SendMessage(hwndProduct, WM_SETFONT, (WPARAM)_hBoldFont, 0);

        TCHAR szProductvVersion1[MAX_LOADSTRING];
        GetDlgItemText(hDlg, IDC_QUNO_VERSION, szProductvVersion1, MAX_LOADSTRING);

        TCHAR szModule[MAX_PATH];
        GetModuleFileName(NULL, szModule, MAX_PATH);

        TCHAR szVersion[MAX_LOADSTRING];
        bResult = GetFileVersionString(szModule, (LPTSTR)Q_FILEVERSION, szVersion, MAX_LOADSTRING);

        if (bResult)
        {
            TCHAR szProductvVersion2[MAX_LOADSTRING];
            wsprintf(szProductvVersion2, szProductvVersion1, szVersion);
            SetDlgItemText(hDlg, IDC_QUNO_VERSION, szProductvVersion2);
        }

        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            DeleteObject(_hBoldFont);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        break;
    }

    return (INT_PTR)FALSE;
}

BOOL GetFileVersionString(LPTSTR lpModule, LPTSTR lpKey, LPTSTR lpValue, UINT uMaxValue)
{
    BOOL bReturn = FALSE;

    DWORD dwSize = GetFileVersionInfoSize(lpModule, 0);

    if (dwSize > 0)
    {
        LPVOID pBlock = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, dwSize);

        if (pBlock)
        {
            BOOL bResult = GetFileVersionInfo(lpModule, 0, dwSize, pBlock);

            if (bResult)
            {
                LANGUAGEANDCODEPAGE* pLanguage;
                UINT uLanguageLen = sizeof(LANGUAGEANDCODEPAGE);
                bResult = VerQueryValue(pBlock, _T("\\VarFileInfo\\Translation"), (LPVOID*)&pLanguage, &uLanguageLen);

                if (bResult && (uLanguageLen > 0))
                {
                    TCHAR szSubBlock[MAX_PATH];
                    wsprintf(szSubBlock, _T("\\StringFileInfo\\%04x%04x\\"), pLanguage[0].wLanguage, pLanguage[0].wCodePage);
                    lstrcat(szSubBlock, lpKey);

                    LPTSTR lpBuffer;
                    bResult = VerQueryValue(pBlock, szSubBlock, (LPVOID*)&lpBuffer, &uMaxValue);

                    if (bResult && (uMaxValue > 0))
                    {
                        lstrcpy(lpValue, lpBuffer);
                        bReturn = TRUE;
                    }
                }
            }

            GlobalFree(GlobalHandle(pBlock));
        }
    }

    return bReturn;
}
