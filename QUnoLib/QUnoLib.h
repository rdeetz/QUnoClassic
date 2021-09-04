// QUnoLib - QUnoLib.h
// 2021 Roger Deetz

#pragma once

#ifdef QUNOLIB_EXPORTS
#define QUNOLIB_API __declspec(dllexport)
#else
#define QUNOLIB_API __declspec(dllimport)
#endif // QUNOLIB_EXPORTS

extern "C" QUNOLIB_API INT CreateGame(INT);

/*
extern "C" QUNOLIB_API INT CreatePlayer(INT);
extern "C" QUNOLIB_API INT CreateDeck(INT);
extern "C" QUNOLIB_API INT AddPlayerToGame(INT);
extern "C" QUNOLIB_API INT StartGame(INT); // shuffle and deal
extern "C" QUNOLIB_API INT CanPlayCard(INT);
extern "C" QUNOLIB_API INT PlayCard(INT);
extern "C" QUNOLIB_API INT PlayWildCard(INT);
extern "C" QUNOLIB_API INT DrawCard(INT);
extern "C" QUNOLIB_API INT GetGameStatus(INT); // current player, wild color, etc
extern "C" QUNOLIB_API INT IsGameOver(INT);
extern "C" QUNOLIB_API INT GetGameInfo(INT); // list of players, etc

extern "C" QUNOLIB_API INT Shuffle(INT);
extern "C" QUNOLIB_API INT Deal(INT);

extern "C" QUNOLIB_API INT ChooseCardToPlay(INT);
extern "C" QUNOLIB_API INT ChooseWildColor(INT);
extern "C" QUNOLIB_API INT AddCardToHand(INT);
extern "C" QUNOLIB_API INT RemoveCardFromHand(INT);
*/
