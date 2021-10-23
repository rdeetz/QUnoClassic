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
    if (hGame == NULL)
    {
        return FALSE;
    }
    
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
    if (lpPlayerName == NULL)
    {
        // Should I call SetLastError here to report the problem?
        return NULL;
    }

    AcquireProcessHeap();

    HPLAYER player = (HPLAYER)HeapAlloc(_hProcessHeap, HEAP_ZERO_MEMORY, sizeof(PLAYER));
    lstrcpy(player->szPlayerName, lpPlayerName);
    player->bIsHuman = bIsHuman;

    return player;
}

BOOL AddPlayerToGame(HGAME hGame, HPLAYER hPlayer, INT nPlayerIndex)
{
    if ((hGame == NULL) ||
        (hPlayer == NULL) ||
        (nPlayerIndex > (GAME_PLAYERS_MAX - 1)))
    {
        return FALSE;
    }

    hGame->players[nPlayerIndex] = hPlayer;
    hGame->nPlayerCount++;

    return TRUE;
}

BOOL RemovePlayerFromGame(HGAME hGame, INT nPlayerIndex)
{
    // Would it be better for this function to return player being removed?
    if ((hGame == NULL) ||
        (nPlayerIndex > (GAME_PLAYERS_MAX - 1)) ||
        (nPlayerIndex < 0))
    {
        return FALSE;
    }

    if (&hGame->players[nPlayerIndex] != NULL)
    {
        return FALSE;
    }

    HeapFree(_hProcessHeap, 0, (LPVOID)hGame->players[nPlayerIndex]);
    hGame->players[nPlayerIndex] = NULL;
    hGame->nPlayerCount--;

    return TRUE;
}

BOOL AddCardToPlayer(HPLAYER hPlayer, HCARD hCard)
{
    if ((hPlayer == NULL) ||
        (hCard == NULL))
    {
        return FALSE;
    }

    for (int i = 0; i < PLAYER_CARDS_MAX; i++)
    {
        // Find the first empty space for the new card;
        if (hPlayer->cards[i] == NULL)
        {
            hPlayer->cards[i] = hCard;
            break;
        }        
    }
    
    hPlayer->nCardCount++;

    return TRUE;
}

BOOL RemoveCardFromPlayer(HPLAYER hPlayer, INT nCardIndex, HCARD* phCard)
{
    if ((hPlayer == NULL) ||
        (nCardIndex > PLAYER_CARDS_MAX) ||
        (nCardIndex < 0))
    {
        phCard = NULL;
        return FALSE;
    }

    *phCard = hPlayer->cards[nCardIndex];
    hPlayer->cards[nCardIndex] = NULL;
    hPlayer->nCardCount--;

    return TRUE;
}

BOOL DealGame(HGAME hGame)
{
    if (hGame == NULL)
    {
        return FALSE;
    }

    return TRUE;
}

VOID AcquireProcessHeap()
{
    if (_hProcessHeap == NULL)
    {
        _hProcessHeap = GetProcessHeap();
    }

    return;
}
