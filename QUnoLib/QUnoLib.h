// QUnoLib - QUnoLib.h
// 2021 Roger Deetz

#pragma once

#ifdef QUNOLIB_EXPORTS
#define QUNOLIB_API __declspec(dllexport)
#else
#define QUNOLIB_API __declspec(dllimport)
#endif // QUNOLIB_EXPORTS

#define PLAYER_NAME_MAX     128
#define PLAYER_CARDS_MAX    16
#define GAME_PLAYERS_MAX    9

#define CARD_COLOR_WILD     0
#define CARD_COLOR_RED      1
#define CARD_COLOR_BLUE     2
#define CARD_COLOR_YELLOW   3
#define CARD_COLOR_GREEN    4

#define CARD_VALUE_ZERO     0
#define CARD_VALUE_ONE      1
#define CARD_VALUE_TWO      2
#define CARD_VALUE_THREE    3
#define CARD_VALUE_FOUR     4
#define CARD_VALUE_FIVE     5
#define CARD_VALUE_SIX      6
#define CARD_VALUE_SEVEN    7
#define CARD_VALUE_EIGHT    8
#define CARD_VALUE_NINE     9
#define CARD_VALUE_SKIP     10
#define CARD_VALUE_REVERSE  11
#define CARD_VALUE_DRAWTWO  12
#define CARD_VALUE_WILD     13
#define CARD_VALUE_WILDD4   14

typedef struct _tagCARD
{
    UINT8 value;
    UINT8 color;
} CARD;

typedef struct _tagPLAYER
{
    TCHAR szPlayerName[PLAYER_NAME_MAX];
    BOOL bIsHuman;
    CARD cards[PLAYER_CARDS_MAX];
    INT nCardCount;
} PLAYER;

typedef PLAYER* HPLAYER;

typedef struct _tagGAME
{
    // TODO Deck
    HPLAYER players[GAME_PLAYERS_MAX];
    INT nPlayerCount;
} GAME;

typedef GAME* HGAME;

extern "C" QUNOLIB_API HGAME CreateGame();
extern "C" QUNOLIB_API BOOL DestroyGame(HGAME hGame);
extern "C" QUNOLIB_API HPLAYER CreatePlayer(LPTSTR lpPlayerName, BOOL bIsHuman);
extern "C" QUNOLIB_API VOID AddPlayerToGame(HGAME hGame, HPLAYER hPlayer, INT nPlayerIndex);

/*
extern "C" QUNOLIB_API BOOL AddCardToPlayer(HPLAYER hPlayer, CARD card);
extern "C" QUNOLIB_API CARD RemoveCardFromPlayer(HPLAYER hPlayer, INT nCardIndex);
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
