// QUnoLib - QUnoLib.cpp
// 2021 Roger Deetz

#include "pch.h"
#include "QUnoLib.h"

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

HPLAYER CreatePlayer(LPTSTR lpPlayerName, BOOL bIsHuman)
{
    return NULL;
}

BOOL AddCardToPlayer(HPLAYER hPlayer, CARD card)
{
    return TRUE;
}

CARD RemoveCardFromPlayer(HPLAYER hPlayer, INT nCardIndex)
{
    CARD card;
    card.color = CARD_COLOR_WILD;
    card.value = CARD_VALUE_WILD;

    return card;
}

HGAME CreateGame()
{
    return NULL;
}
