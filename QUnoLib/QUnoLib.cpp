// QUnoLib - QUnoLib.cpp
// 2021 Roger Deetz

#include "pch.h"
#include "QUnoLib.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

INT CreateGame(INT nNumberOfPlayers)
{
    INT result = 0;

    if (nNumberOfPlayers <= 0)
    {
        result = -1;
    }
    else if (nNumberOfPlayers > 9)
    {
        result = 0;
    }
    else
    {
        result = 1;
    }

    return result;
}
