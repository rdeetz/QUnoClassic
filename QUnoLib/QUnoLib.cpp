// QUnoLib - QUnoLib.cpp
// 2021 Roger Deetz

#include "pch.h"
#include "QUnoLib.h"
#include <time.h>
#include <random>

HANDLE _hProcessHeap;

VOID AcquireProcessHeap();
VOID ProvideStandardDeck(HGAME hGame);

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

    HGAME hGame = (HGAME)HeapAlloc(_hProcessHeap, HEAP_ZERO_MEMORY, sizeof(GAME));
    ProvideStandardDeck(hGame);

    // For some reason player[0] appears to be corrupted after the call to ProvideStandardDeck.
    hGame->players[0] = NULL;

    return hGame;
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
        if (hGame->players[i] != NULL)
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

INT AddPlayerToGame(HGAME hGame, HPLAYER hPlayer)
{
    if ((hGame == NULL) || (hPlayer == NULL))
    {
        return -1;
    }

    INT nPlayerIndex = 0;

    for (int i = 0; i < GAME_PLAYERS_MAX; i++)
    {
        // Find the first empty location for the new player
        if (hGame->players[i] == NULL)
        {
            hGame->players[i] = hPlayer;
            hGame->nPlayerCount++;
            nPlayerIndex = i;
            break;
        }
    }

    return nPlayerIndex;
}

BOOL RemovePlayerFromGame(HGAME hGame, UINT nPlayerIndex)
{
    if ((hGame == NULL) || (nPlayerIndex > (GAME_PLAYERS_MAX - 1)))
    {
        return FALSE;
    }

    if (hGame->players[nPlayerIndex] == NULL)
    {
        return FALSE;
    }

    HeapFree(_hProcessHeap, 0, (LPVOID)hGame->players[nPlayerIndex]);
    hGame->players[nPlayerIndex] = NULL;
    hGame->nPlayerCount--;

    return TRUE;
}

INT AddCardToPlayer(HPLAYER hPlayer, HCARD hCard)
{
    if ((hPlayer == NULL) || (hCard == NULL))
    {
        return -1;
    }

    INT nCardIndex = 0;

    for (int i = 0; i < PLAYER_CARDS_MAX; i++)
    {
        // Find the first empty location for the new card.
        if (hPlayer->cards[i] == NULL)
        {
            hPlayer->cards[i] = hCard;
            hPlayer->nCardCount++;
            nCardIndex = i;
            break;
        }        
    }    

    return nCardIndex;
}

BOOL RemoveCardFromPlayer(HPLAYER hPlayer, UINT nCardIndex, HCARD* phCard)
{
    if ((hPlayer == NULL) || (nCardIndex > PLAYER_CARDS_MAX))
    {
        phCard = NULL;
        return FALSE;
    }

    if (hPlayer->cards[nCardIndex] == NULL)
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

    // Add all the cards from the deck to the draw pile.
    for (int i = 0; i < GAME_DECK_MAX; i++)
    {
        hGame->drawPile[i] = &(hGame->deck[i]);
    }

    // Shuffle the draw pile.
    time_t seconds = time(NULL);
    srand((unsigned int)seconds);

    for (int i = GAME_DECK_MAX - 1; i > 0; i--)
    {
        int r = rand();
        double ratio = (double)r / INT16_MAX;
        int s = (int) floor(i * ratio);
        HCARD temp = hGame->drawPile[i];
        hGame->drawPile[i] = hGame->drawPile[s];
        hGame->drawPile[s] = temp;
    }

    for (int round = 0; round < PLAYER_CARDS_START; round++)
    {
        for (UINT player = 0; player < hGame->nPlayerCount; player++)
        {
            UINT index = (round * (PLAYER_CARDS_START - 1)) + player;
            HPLAYER hPlayer = hGame->players[player];
            HCARD hCard = hGame->drawPile[index];

            AddCardToPlayer(hPlayer, hCard);
        }
    }

    return TRUE;
}

BOOL CanCardPlay(HGAME hGame, HCARD hCard)
{
    BOOL canPlay = FALSE;
    HCARD hCurrentCard = hGame->discardPile[hGame->nDiscardPileIndex];

    if (hCard->value >= CARD_VALUE_WILD)
    {
        canPlay = TRUE;
    }
    else
    {
        if ((hCard->value == hCurrentCard->value) || 
            ((hCard->color == hCurrentCard->color) || (hCard->color == hGame->nCurrentWildColor)))
        {
            canPlay = TRUE;
        }
    }

    return canPlay;
}

VOID AcquireProcessHeap()
{
    if (_hProcessHeap == NULL)
    {
        _hProcessHeap = GetProcessHeap();
    }

    return;
}

VOID ProvideStandardDeck(HGAME hGame)
{
    CARD c0; c0.color = CARD_COLOR_WILD; c0.value = CARD_VALUE_WILD;
    hGame->deck[0] = c0;
    CARD c1; c1.color = CARD_COLOR_WILD; c1.value = CARD_VALUE_WILD;
    hGame->deck[1] = c1;
    CARD c2; c2.color = CARD_COLOR_WILD; c2.value = CARD_VALUE_WILDD4;
    hGame->deck[2] = c2;
    CARD c3; c3.color = CARD_COLOR_WILD; c3.value = CARD_VALUE_WILDD4;
    hGame->deck[3] = c3;

    int index = 4; // This is starting at 4 since we just added 4 cards.

    for (int c = CARD_COLOR_RED; c <= CARD_COLOR_GREEN; c++)
    {
        for (int v = CARD_VALUE_ZERO; v <= CARD_VALUE_DRAWTWO; v++)
        {
            CARD card;
            card.color = c;
            card.value = v;

            hGame->deck[index] = card;
            index++;
        }
    }

    return;
}
