// QUnoLib - QUnoLib.cpp
// 2021 Roger Deetz

#include "pch.h"
#include "QUnoLib.h"

HANDLE _hProcessHeap;

VOID AcquireProcessHeap();

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
    AcquireProcessHeap();

    return (HGAME)HeapAlloc(_hProcessHeap, HEAP_ZERO_MEMORY, sizeof(GAME));
}

BOOL DestroyGame(HGAME hGame)
{
    AcquireProcessHeap();

    for (int i = 0; i < GAME_PLAYERS_MAX; i++)
    {
        if (&hGame->players[i] != NULL)
        {
            HeapFree(_hProcessHeap, 0, (LPVOID)hGame->players[i]);
        }
    }

    return HeapFree(_hProcessHeap, 0, (LPVOID)hGame);
}

HPLAYER CreatePlayer(LPTSTR lpPlayerName, BOOL bIsHuman)
{
    AcquireProcessHeap();

    HPLAYER player = (HPLAYER)HeapAlloc(_hProcessHeap, HEAP_ZERO_MEMORY, sizeof(PLAYER));
    lstrcpy(player->szPlayerName, lpPlayerName);
    player->bIsHuman = bIsHuman;

    return player;
}

VOID AddPlayerToGame(HGAME hGame, HPLAYER hPlayer, INT nPlayerIndex)
{
    hGame->players[nPlayerIndex] = hPlayer;

    return;
}

VOID DealGame(HGAME hGame)
{
    // TODO Shuffle up and deal.

    return;
}

VOID AcquireProcessHeap()
{
    if (_hProcessHeap == NULL)
    {
        _hProcessHeap = GetProcessHeap();
    }

    return;
}
