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

const COLORREF Q_COLOR_RED      = 0x000000FF;
const COLORREF Q_COLOR_BLUE     = 0x00FF0000;
const COLORREF Q_COLOR_YELLOW   = 0x0000FFFF;
const COLORREF Q_COLOR_GREEN    = 0x0000FF00;
const COLORREF Q_COLOR_BLACK    = 0x00000000;

HINSTANCE _hInstance;
TCHAR _szWindowTitle[MAX_LOADSTRING];
TCHAR _szWindowClass[MAX_LOADSTRING];
HFONT _hBoldFont;
TCHAR _szDefaultPlayerName[MAX_LOADSTRING];
UINT _nDefaultComputerPlayers = 3;
TCHAR _szNewGamePrompt[MAX_LOADSTRING];
TCHAR _szPlayerKindHuman[MAX_LOADSTRING];
TCHAR _szPlayerKindRobot[MAX_LOADSTRING];
HBRUSH _hbrushRed;
HBRUSH _hbrushBlue;
HBRUSH _hbrushYellow;
HBRUSH _hbrushGreen;
HBRUSH _hbrushBlack;
HGAME _hCurrentGame;

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

    // This needs to come before InitInstance since the string is used in the initial painting.
    LoadString(hInstance, IDS_NEWGAMEPROMPT, _szNewGamePrompt, MAX_LOADSTRING);
    LoadString(hInstance, IDS_PLAYER_HUMAN, _szPlayerKindHuman, MAX_LOADSTRING);
    LoadString(hInstance, IDS_PLAYER_ROBOT, _szPlayerKindRobot, MAX_LOADSTRING);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    _hbrushRed = CreateSolidBrush(Q_COLOR_RED);
    _hbrushBlue = CreateSolidBrush(Q_COLOR_BLUE);
    _hbrushYellow = CreateSolidBrush(Q_COLOR_YELLOW);
    _hbrushGreen = CreateSolidBrush(Q_COLOR_GREEN);
    _hbrushBlack = CreateSolidBrush(Q_COLOR_BLACK);

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
       WS_OVERLAPPEDWINDOW | WS_EX_CLIENTEDGE, 
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
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case IDM_END:
            StopGame();
            InvalidateRect(hWnd, NULL, TRUE);
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

        if (_hCurrentGame != NULL)
        {
            // TODO Game in progress, draw it.
            //      Show each player and their cards.
            //      Show the draw and discard piles for the game.
            //      Will need to capture the current player, direction, and wild color for the game.
            RECT rcClient;
            GetClientRect(hWnd, &rcClient);
            LONG left = rcClient.left + 128;

            for (UINT player = 0; player < GAME_PLAYERS_MAX; player++)
            {
                HPLAYER hPlayer = _hCurrentGame->players[player];
                
                if (hPlayer != NULL)
                {
                    SIZE size;
                    GetTextExtentPoint32(hdc, hPlayer->szPlayerName, lstrlen(hPlayer->szPlayerName), &size);
                    LONG top = rcClient.top + size.cy;
                    TextOut(hdc, left, top, hPlayer->szPlayerName, lstrlen(hPlayer->szPlayerName));
                    LPTSTR lpPlayerKind = hPlayer->bIsHuman ? _szPlayerKindHuman : _szPlayerKindRobot;
                    top += size.cy;
                    TextOut(hdc, left, top, lpPlayerKind, lstrlen(lpPlayerKind));

                    for (UINT card = 0; card < PLAYER_CARDS_MAX; card++)
                    {
                        HCARD hCard = hPlayer->cards[card];

                        if (hCard != NULL)
                        {
                            top += size.cy;
                            TCHAR szCard[MAX_LOADSTRING];
                            wsprintf(szCard, L"Color: %d, Value: %d", hCard->color, hCard->value);
                            TextOut(hdc, left, top, szCard, lstrlen(szCard));
                            RECT rcCard;
                            rcCard.left = left;
                            rcCard.top = top;
                            rcCard.right = left + size.cx;
                            rcCard.bottom = top + size.cy;

                            switch (hCard->color)
                            {
                            case CARD_COLOR_RED:
                                FillRect(hdc, &rcCard, _hbrushRed);
                                break;
                            case CARD_COLOR_BLUE:
                                FillRect(hdc, &rcCard, _hbrushBlue);
                                break;
                            case CARD_COLOR_YELLOW:
                                FillRect(hdc, &rcCard, _hbrushYellow);
                                break;
                            case CARD_COLOR_GREEN:
                                FillRect(hdc, &rcCard, _hbrushGreen);
                                break;
                            case CARD_COLOR_WILD:
                                FillRect(hdc, &rcCard, _hbrushBlack);
                                break;
                            default:
                                break;
                            }
                        }
                    }

                    left += (size.cx + 128); // 64 is a guess for a nice padding.
                }
            }
        }
        else
        {
            RECT rcClient;
            GetClientRect(hWnd, &rcClient);
            SIZE size;
            GetTextExtentPoint32(hdc, _szNewGamePrompt, lstrlen(_szNewGamePrompt), &size);
            TextOut(hdc, 
                rcClient.left + (((rcClient.right - rcClient.left) / 2) - (size.cx / 2)), 
                rcClient.top + (((rcClient.bottom - rcClient.top) / 2) - (size.cy / 2)), 
                _szNewGamePrompt, 
                lstrlen(_szNewGamePrompt));
        }

        EndPaint(hWnd, &ps);
    }
        break;

    case WM_DESTROY:
        DeleteObject(_hbrushRed);
        DeleteObject(_hbrushBlue);
        DeleteObject(_hbrushGreen);
        DeleteObject(_hbrushYellow);
        DeleteObject(_hbrushBlack);
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
    if (_hCurrentGame == NULL)
    {
        StopGame();
    }

    _hCurrentGame = CreateGame();

    HPLAYER hHumanPlayer = CreatePlayer(_szDefaultPlayerName, TRUE);
    AddPlayerToGame(_hCurrentGame, hHumanPlayer);

    TCHAR szPlayerNameTemplate[MAX_LOADSTRING];
    LoadString(_hInstance, IDS_TEMPLATEPLAYERNAME, szPlayerNameTemplate, MAX_LOADSTRING);

    for (UINT i = 1; i <= _nDefaultComputerPlayers; i++)
    {
        TCHAR szComputerPlayerName[MAX_LOADSTRING];
        wsprintf(szComputerPlayerName, szPlayerNameTemplate, i + 1); // This isn't great.

        HPLAYER hComputerPlayer = CreatePlayer(szComputerPlayerName, FALSE);
        AddPlayerToGame(_hCurrentGame, hComputerPlayer);
    }

    DealGame(_hCurrentGame);
    
    return;
}

VOID StopGame()
{
    DestroyGame(_hCurrentGame);
    _hCurrentGame = NULL;

    return;
}
