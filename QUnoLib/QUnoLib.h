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
#define PLAYER_CARDS_START  5
#define GAME_PLAYERS_MAX    9
#define GAME_DECK_MAX       ((12 * 4) + 4) // 1 each for each color, 2 wild, 2 wild-draw-four

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

#define DIRECTION_CLOCKWISE         0
#define DIRECTION_COUNTERCLOCKWISE  1

typedef struct _CARD
{
    UINT8 color;
    UINT8 value;
} CARD;

typedef CARD* HCARD;

typedef struct _PLAYER
{
    TCHAR szPlayerName[PLAYER_NAME_MAX];
    BOOL bIsHuman;
    HCARD cards[PLAYER_CARDS_MAX];
    UINT nCardCount;
} PLAYER;

typedef PLAYER* HPLAYER;

typedef struct _GAME
{
    CARD deck[GAME_DECK_MAX];
    HPLAYER players[GAME_PLAYERS_MAX];
    UINT nPlayerCount;
    HCARD drawPile[GAME_DECK_MAX];
    UINT nDrawPileIndex;
    HCARD discardPile[GAME_DECK_MAX];
    UINT nDiscardPileIndex;
    UINT8 nCurrentDirection;
    UINT8 nCurrentWildColor;
    UINT nCurrentPlayerIndex;
} GAME;

typedef GAME* HGAME;

extern "C" QUNOLIB_API HGAME CreateGame();
extern "C" QUNOLIB_API BOOL DestroyGame(HGAME hGame);
extern "C" QUNOLIB_API HPLAYER CreatePlayer(LPTSTR lpPlayerName, BOOL bIsHuman);
extern "C" QUNOLIB_API INT AddPlayerToGame(HGAME hGame, HPLAYER hPlayer);
extern "C" QUNOLIB_API BOOL RemovePlayerFromGame(HGAME hGame, UINT nPlayerIndex); // Should this return the player?
extern "C" QUNOLIB_API INT AddCardToPlayer(HPLAYER hPlayer, HCARD hCard);
extern "C" QUNOLIB_API BOOL RemoveCardFromPlayer(HPLAYER hPlayer, UINT nCardIndex, HCARD* phCard);
extern "C" QUNOLIB_API BOOL DealGame(HGAME hGame);
extern "C" QUNOLIB_API BOOL CanCardPlay(HGAME hGame, HCARD hCard);
