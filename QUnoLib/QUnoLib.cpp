// QUnoLib - QUnoLib.cpp
// 2021 Roger Deetz

#include "pch.h"
#include "QUnoLib.h"

HANDLE _hProcessHeap;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

HGAME CreateGame()
{
    if (_hProcessHeap == NULL)
    {
        _hProcessHeap = GetProcessHeap();
    }

    return (HGAME)HeapAlloc(_hProcessHeap, HEAP_ZERO_MEMORY, sizeof(GAME));
}

BOOL DestroyGame(HGAME hGame)
{
    if (_hProcessHeap == NULL)
    {
        _hProcessHeap = GetProcessHeap();
    }

    /*
    for (int i = 0; i < GAME_PLAYERS_MAX; i++)
    {
        if (&hGame->players[i] != NULL)
        {
            HeapFree(hHeap, 0, &hGame->players[i]);
        }
    }
    */

    return HeapFree(_hProcessHeap, 0, (LPVOID)hGame);
}

/*
HPLAYER CreatePlayer(LPTSTR lpPlayerName, BOOL bIsHuman)
{
    HANDLE hHeap = GetProcessHeap();
    HPLAYER player = (HPLAYER)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(PLAYER));

    lstrcpy(player->szPlayerName, lpPlayerName);
    player->bIsHuman = bIsHuman;

    return player;
}

VOID AddPlayerToGame(HGAME hGame, HPLAYER hPlayer)
{
    for (int i = 0; i < GAME_PLAYERS_MAX; i++)
    {
        if (&hGame->players[i] == NULL)
        {
            hGame->players[i] = *hPlayer;
        }
    }

    return;
}
*/
