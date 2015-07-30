/**
 * @file include/BugSquasher2/gameCtx.h
 * 
 * The game's "global" context
 */
#ifndef __GAMECTX_H__
#define __GAMECTX_H__

#include <GFraMe/gframe.h>
#include <GFraMe/gfmSpriteset.h>

/**  Define the possible game states */
enum enGameState {
    game_mainMenu = 0,
    game_playState,
    game_maxState,
};
typedef enum enGameState gameState;

/** Main menu struct */
typedef struct stMainMenuCtx mainMenuCtx;

/** Struct used to keep the game context and all it's "static" stuff (like 
 * assets and such...) */
struct stGameCtx {
    /** Currently playing state */
    gameState state;
    /** The actualy library context */
    gfmCtx *pCtx;
    /** 8x8 spriteset */
    gfmSpriteset *pSset8x8;
    /** 8x16 spriteset */
    gfmSpriteset *pSset8x16;
    /** 16x16 spriteset */
    gfmSpriteset *pSset16x16;
    /** 32x32 spriteset */
    gfmSpriteset *pSset32x32;
    /** Handle of the action key */
    int actionHnd;
    /** Handle for saving animated GIFs */
    int gifHnd;
    /** Whether the game is running or something stopped it (code-wise) */
    int isRunning;
    /** Texture's handle */
    int iTex;
    /** Flag that the state should change */
    int switchState;
    /** The main menu context */
    mainMenuCtx *pMainMenu;
};
typedef struct stGameCtx gameCtx;

#endif /* __GAMECTX_H__ */

