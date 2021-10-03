// QUnoClassic - QUnoClassic.cpp
// 2021 Roger Deetz

#include "framework.h"
#include "QUnoClassic.h"
#include "QUnoLib.h"

#define MAX_LOADSTRING                              128
#define Q_FILEVERSION                               L"FileVersion"
#define Q_MODULE_LIB                                L"QUNOLIB.DLL"
#define Q_WINDOW_WIDTH_DEFAULT                      1024
#define Q_WINDOW_HEIGHT_DEFAULT                     768
#define Q_WINDOW_WIDTH_MIN                          500
#define Q_WINDOW_HEIGHT_MIN                         375
#define Q_REGISTRY_KEY_ROOT                         L"Software\\Mooville\\QUno\\2.5"
#define Q_REGISTRY_VALUE_DEFAULTPLAYERNAME          L"DefaultPlayerName"
#define Q_REGISTRY_VALUE_DEFAULTCOMPUTERPLAYERS     L"DefaultComputerPlayers"

struct LANGUAGEANDCODEPAGE
{
    WORD wLanguage;
    WORD wCodePage;
};

HINSTANCE _hInstance;
TCHAR _szWindowTitle[MAX_LOADSTRING];
TCHAR _szWindowClass[MAX_LOADSTRING];
HFONT _hBoldFont;
TCHAR _szDefaultPlayerName[MAX_LOADSTRING];
UINT _nDefaultComputerPlayers = 3;
HGAME _hActiveGame;

ATOM RegisterWndClass(HINSTANCE);
BOOL InitInstance(HINSTANCE, INT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK OptionsDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
VOID CenterDialogBox(HWND);
VOID SetDlgItemVersion(HWND, UINT, LPTSTR);
BOOL GetFileVersionString(LPTSTR, LPTSTR, LPTSTR, UINT);
BOOL IsDefaultPlayerNameValid(HWND);
BOOL IsDefaultComputerPlayersValid(HWND);
VOID StartGame();
VOID StopGame();

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPreviousInstance, _In_ LPTSTR lpCmdLine, _In_ INT nCmdShow)
{
    UNREFERENCED_PARAMETER(hPreviousInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadString(hInstance, IDS_QUNO, _szWindowTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_QUNOCLASSIC, _szWindowClass, MAX_LOADSTRING);
    RegisterWndClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HKEY hKey;
    RegCreateKey(HKEY_CURRENT_USER, Q_REGISTRY_KEY_ROOT, &hKey);

    DWORD cbString = sizeof(_szDefaultPlayerName);
    LSTATUS lResult = RegGetValue(hKey, NULL, Q_REGISTRY_VALUE_DEFAULTPLAYERNAME, RRF_RT_REG_SZ, NULL, _szDefaultPlayerName, &cbString);

    if (lResult != ERROR_SUCCESS)
    {
        LoadString(hInstance, IDS_DEFAULTPLAYERNAME, _szDefaultPlayerName, MAX_LOADSTRING);
    }

    DWORD cbUint = sizeof(UINT);
    lResult = RegGetValue(hKey, NULL, Q_REGISTRY_VALUE_DEFAULTCOMPUTERPLAYERS, RRF_RT_REG_DWORD, NULL, &_nDefaultComputerPlayers, &cbUint);

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

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, INT nCmdShow)
{
   _hInstance = hInstance;

   HWND hWnd = CreateWindow(
       _szWindowClass, 
       _szWindowTitle, 
       WS_OVERLAPPEDWINDOW, 
       CW_USEDEFAULT, 
       CW_USEDEFAULT, 
       Q_WINDOW_WIDTH_DEFAULT, 
       Q_WINDOW_HEIGHT_DEFAULT, 
       NULL, 
       NULL, 
       hInstance, 
       NULL);

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
    LPRECT lpRect;
    UINT nCurrentWidth;
    UINT nCurrentHeight;

    switch (message)
    {
    case WM_COMMAND:
    {
        INT wmId = LOWORD(wParam);

        switch (wmId)
        {
        case IDM_NEW:
            StartGame();
            break;

        case IDM_OPTIONS:
            DialogBox(_hInstance, MAKEINTRESOURCE(IDD_OPTIONSDLG), hWnd, OptionsDlgProc);
            break;

        case IDM_ABOUT:
            DialogBox(_hInstance, MAKEINTRESOURCE(IDD_ABOUTDLG), hWnd, AboutDlgProc);
            break;

        case IDM_EXIT:
            StopGame();
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
        break;

    case WM_SIZING:
        lpRect = (LPRECT)lParam;
        nCurrentWidth = lpRect->right - lpRect->left;
        nCurrentHeight = lpRect->bottom - lpRect->top;

        if (nCurrentWidth <= Q_WINDOW_WIDTH_MIN)
        {
            lpRect->right = lpRect->left + Q_WINDOW_WIDTH_MIN;
        }

        if (nCurrentHeight <= Q_WINDOW_HEIGHT_MIN)
        {
            lpRect->bottom = lpRect->top + Q_WINDOW_HEIGHT_MIN;
        }

        return TRUE;

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
        CenterDialogBox(hDlg);

        // Create an up-down control rather than a plain edit control 
        // for the default number of computer players.
        // https://docs.microsoft.com/en-us/windows/win32/controls/create-an-up-down-control

        SetDlgItemText(hDlg, IDC_DEFAULTPLAYERNAME, _szDefaultPlayerName);
        SetDlgItemInt(hDlg, IDC_DEFAULTCOMPUTERPLAYERS, _nDefaultComputerPlayers, FALSE);

        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            if (IsDefaultPlayerNameValid(hDlg) && IsDefaultComputerPlayersValid(hDlg))
            {
                HKEY hKey;
                RegCreateKey(HKEY_CURRENT_USER, Q_REGISTRY_KEY_ROOT, &hKey);

                DWORD cbString = sizeof(_szDefaultPlayerName);
                LSTATUS lResult = RegSetValueEx(hKey, Q_REGISTRY_VALUE_DEFAULTPLAYERNAME, 0, REG_SZ, (BYTE*)_szDefaultPlayerName, cbString);
                DWORD cbUint = sizeof(UINT);
                lResult = RegSetValueEx(hKey, Q_REGISTRY_VALUE_DEFAULTCOMPUTERPLAYERS, 0, REG_DWORD, (BYTE*)&_nDefaultComputerPlayers, cbUint);

                EndDialog(hDlg, LOWORD(wParam));
            }
            else
            {
                TCHAR szError[MAX_LOADSTRING];
                LoadString(_hInstance, IDS_ERROR_OPTIONS, szError, MAX_LOADSTRING);
                MessageBox(hDlg, szError, _szWindowTitle, MB_ICONWARNING | MB_OK);
            }

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

BOOL IsDefaultPlayerNameValid(HWND hDlg)
{
    BOOL bIsValid = FALSE;
    TCHAR szDefaultPlayerName[MAX_LOADSTRING];
    GetDlgItemText(hDlg, IDC_DEFAULTPLAYERNAME, szDefaultPlayerName, MAX_LOADSTRING);

    if (lstrlen(szDefaultPlayerName) > 0)
    {
        lstrcpy(_szDefaultPlayerName, szDefaultPlayerName);
        bIsValid = TRUE;
    }

    return bIsValid;
}

BOOL IsDefaultComputerPlayersValid(HWND hDlg)
{
    BOOL bIsValid = FALSE;
    BOOL bTranslated;
    UINT nResult = GetDlgItemInt(hDlg, IDC_DEFAULTCOMPUTERPLAYERS, &bTranslated, FALSE);

    if ((bTranslated == TRUE) && ((nResult > 0) && (nResult <= GAME_PLAYERS_MAX)))
    {
        _nDefaultComputerPlayers = nResult;
        bIsValid = TRUE;
    }

    return bIsValid;
}

INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    HWND hwndProduct;
    HFONT hOldFont;
    LOGFONT lf;

    switch (message)
    {
    case WM_INITDIALOG:
        CenterDialogBox(hDlg);

        hwndProduct = GetDlgItem(hDlg, IDC_QUNO_PRODUCT);
        hOldFont = (HFONT)SendMessage(hwndProduct, WM_GETFONT, 0, 0);
        GetObject(hOldFont, sizeof(LOGFONT), (LPVOID)&lf);
        lf.lfWeight = FW_BOLD;
        _hBoldFont = CreateFontIndirect(&lf);
        SendMessage(hwndProduct, WM_SETFONT, (WPARAM)_hBoldFont, 0);

        TCHAR szModule[MAX_PATH];
        GetModuleFileName(NULL, szModule, MAX_PATH);
        SetDlgItemVersion(hDlg, IDC_QUNO_VERSION, szModule);
        SetDlgItemVersion(hDlg, IDC_QUNOLIB_VERSION, (LPTSTR)Q_MODULE_LIB);

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

VOID CenterDialogBox(HWND hDlg)
{
    RECT rcOwner;
    RECT rcDlg;
    RECT rcNew;

    HWND hwndOwner = GetParent(hDlg);
    GetWindowRect(hwndOwner, &rcOwner);
    GetWindowRect(hDlg, &rcDlg);
    
    CopyRect(&rcNew, &rcOwner);
    OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
    OffsetRect(&rcNew, -rcNew.left, -rcNew.top);
    OffsetRect(&rcNew, -rcDlg.right, -rcDlg.bottom);

    SetWindowPos(hDlg, HWND_TOP, rcOwner.left + (rcNew.right / 2), rcOwner.top + (rcNew.bottom / 2), 0, 0, SWP_NOSIZE);

    return;
}

VOID SetDlgItemVersion(HWND hDlg, UINT nDlgItem, LPTSTR lpModule)
{
    TCHAR szTemplate[MAX_LOADSTRING];
    GetDlgItemText(hDlg, nDlgItem, szTemplate, MAX_LOADSTRING);

    TCHAR szVersion[MAX_LOADSTRING];
    BOOL bResult = GetFileVersionString(lpModule, (LPTSTR)Q_FILEVERSION, szVersion, MAX_LOADSTRING);

    if (bResult)
    {
        TCHAR szFinal[MAX_LOADSTRING];
        wsprintf(szFinal, szTemplate, szVersion);
        SetDlgItemText(hDlg, nDlgItem, szFinal);
    }

    return;
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

VOID StartGame()
{
    _hActiveGame = CreateGame();

    HPLAYER hHumanPlayer = CreatePlayer(_szDefaultPlayerName, TRUE);
    AddPlayerToGame(_hActiveGame, hHumanPlayer, 0);

    TCHAR szPlayerNameTemplate[MAX_LOADSTRING];
    LoadString(_hInstance, IDS_TEMPLATEPLAYERNAME, szPlayerNameTemplate, MAX_LOADSTRING);

    for (UINT i = 0; i < _nDefaultComputerPlayers - 1; i++)
    {
        TCHAR szComputerPlayerName[MAX_LOADSTRING];
        wsprintf(szComputerPlayerName, szPlayerNameTemplate, i + 2);

        HPLAYER hComputerPlayer = CreatePlayer(szComputerPlayerName, FALSE);
        AddPlayerToGame(_hActiveGame, hComputerPlayer, i + 1);
    }

    // TODO Deal the game.
    
    return;
}

VOID StopGame()
{
    DestroyGame(_hActiveGame);

    return;
}
