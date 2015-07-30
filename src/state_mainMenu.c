/**
 * @file src/state_mainMenu.c
 * 
 * The game's main menu
 */
#include <BugSquasher2/gameCtx.h>
#include <BugSquasher2/state_mainMenu.h>

#include <GFraMe/gframe.h>
#include <GFraMe/gfmAssert.h>
#include <GFraMe/gfmError.h>
#include <GFraMe/gfmSpriteset.h>
#include <GFraMe/gfmTilemap.h>

#include <stdlib.h>
#include <string.h>

/** Title data for the tilemap */
int titleData[] = {
#include "../assets/title.csv"
};
/** Backgroun data for the tilemap */
int backgroundData[] = {
#include "../assets/background.csv"
};

/** Main menu struct */
struct stMainMenuCtx {
    /** A tilemap to show the title, in 8x8 tiles */
    gfmTilemap *pTitle;
    /** A tilemap to show the background, in 8x8 tiles */
    gfmTilemap *pBackground;
    /** Current position in the "title animation" (copying of the tilemap) */
    int titleAnimation;
    /** Accumulator for the animation */
    int animationTime;
};

/**
 * Initialize everything needed by the main menu
 * 
 * @param  pGame The game "global" context
 * @return       GFMRV_OK, GFMRV_ARGUMENTS_BAD, GFMRV_ALLOC_FAILED, ...
 */
static gfmRV state_mainMenu_init(gameCtx *pGame) {
    gfmRV rv;
    mainMenuCtx *pMainMenu;
    
    // Sanitize arguments
    ASSERT(pGame, GFMRV_ARGUMENTS_BAD);
    
    // Alloc the main menu
    pGame->pMainMenu = (mainMenuCtx*)malloc(sizeof(mainMenuCtx));
    ASSERT(pGame->pMainMenu, GFMRV_ALLOC_FAILED);
    pMainMenu = pGame->pMainMenu;
    // Clean it
    memset(pMainMenu, 0x0, sizeof(mainMenuCtx));
    
    // Alloc and initialize the title
    rv = gfmTilemap_getNew(&(pMainMenu->pTitle));
    ASSERT_NR(rv == GFMRV_OK);
    rv = gfmTilemap_init(pMainMenu->pTitle, pGame->pSset8x8, 32, 16, -1);
    ASSERT_NR(rv == GFMRV_OK);
    rv = gfmTilemap_setPosition(pMainMenu->pTitle, 32, 0);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Alloc and load the background
    rv = gfmTilemap_getNew(&(pMainMenu->pBackground));
    ASSERT_NR(rv == GFMRV_OK);
    rv = gfmTilemap_init(pMainMenu->pBackground, pGame->pSset8x16, 40, 8, -1);
    ASSERT_NR(rv == GFMRV_OK);
    rv = gfmTilemap_loadStatic(pMainMenu->pBackground, backgroundData, 40, 8);
    ASSERT_NR(rv == GFMRV_OK);
    rv = gfmTilemap_setPosition(pMainMenu->pBackground, 0, 56);
    ASSERT_NR(rv == GFMRV_OK);
    
    // TODO initialize everything else
    
    rv = GFMRV_OK;
__ret:
    return rv;
}

/**
 * Clean everything used by the main menu
 * 
 * @param  pGame The game "global" context
 * @return       GFMRV_OK, GFMRV_ARGUMENTS_BAD, GFMRV_ALLOC_FAILED, ...
 */
static gfmRV state_mainMenu_clean(gameCtx *pGame) {
    gfmRV rv;
    
    // Sanitize arguments
    ASSERT(pGame, GFMRV_ARGUMENTS_BAD);
    // Check that it was initialized
    ASSERT(pGame->pMainMenu, GFMRV_OK);
    
    // Clean the title
    gfmTilemap_free(&(pGame->pMainMenu->pTitle));
    // Clean the background
    gfmTilemap_free(&(pGame->pMainMenu->pBackground));
    // TODO Clean everything
    
    // Dealloc and clean the main menu
    free(pGame->pMainMenu);
    pGame->pMainMenu = 0;
    
    rv = GFMRV_OK;
__ret:
    return rv;
}

/**
 * Updates everything in this state
 * 
 * @param  pGame The game "global" context
 */
static gfmRV state_mainMenu_update(gameCtx *pGame) {
    gfmRV rv;
    int elapsed;
    mainMenuCtx *pMainMenu;
    
    // Sanitize arguments
    ASSERT(pGame, GFMRV_ARGUMENTS_BAD);
    ASSERT(pGame->pMainMenu, GFMRV_ARGUMENTS_BAD);
    // Retrieve the main menu context
    pMainMenu = pGame->pMainMenu;
    
    // Get how long elapsed since the last frame
    rv = gfm_getElapsedTime(&elapsed, pGame->pCtx);
    ASSERT_NR(rv == GFMRV_OK);
    
    pMainMenu->animationTime += elapsed;
    while (pMainMenu->animationTime > 16) {
        // Get the tile data and animate it
        if (pMainMenu->titleAnimation < sizeof(titleData) / sizeof(int)) {
            int *pData;
            
            // Go to the next valid (i.e., non-invisible) tile
            while (pMainMenu->titleAnimation + 1 < sizeof(titleData) &&
                    titleData[pMainMenu->titleAnimation] == -1) {
                pMainMenu->titleAnimation++;
            }
            
            // Get its data
            rv = gfmTilemap_getData(&pData, pMainMenu->pTitle);
            ASSERT_NR(rv == GFMRV_OK);
            // Update the tile
            pData[pMainMenu->titleAnimation] = titleData[pMainMenu->titleAnimation];
            
            pMainMenu->titleAnimation++;
        }
        pMainMenu->animationTime -= 16;
    }
    
    rv = GFMRV_OK;
__ret:
    return rv;
}

/**
 * Draw everything in this state
 * 
 * @param  pGame The game "global" context
 */
static gfmRV state_mainMenu_draw(gameCtx *pGame) {
    gfmRV rv;
    mainMenuCtx *pMainMenu;
    
    // Sanitize arguments
    ASSERT(pGame, GFMRV_ARGUMENTS_BAD);
    ASSERT(pGame->pMainMenu, GFMRV_ARGUMENTS_BAD);
    // Retrieve the main menu context
    pMainMenu = pGame->pMainMenu;
    
    // Draw the background
    rv = gfmTilemap_draw(pMainMenu->pBackground, pGame->pCtx);
    ASSERT_NR(rv == GFMRV_OK);
    // Draw the title
    rv = gfmTilemap_draw(pMainMenu->pTitle, pGame->pCtx);
    ASSERT_NR(rv == GFMRV_OK);
    
    rv = GFMRV_OK;
__ret:
    return rv;
}

/**
 * The game's main menu
 * 
 * @param  pGame The game "global" context
 * @return       GFMRV_OK, GFMRV_ARGUMENTS_BAD, GFMRV_ALLOC_FAILED, ...
 */
gfmRV state_mainMenu(gameCtx *pGame) {
    gfmRV rv;
    
    // Sanitize arguments
    ASSERT(pGame, GFMRV_ARGUMENTS_BAD);
    
    // Initialize the main menu
    rv = state_mainMenu_init(pGame);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Run until the window is closed
    while (pGame->isRunning && gfm_didGetQuitFlag(pGame->pCtx) == GFMRV_FALSE) {
        rv = gfm_handleEvents(pGame->pCtx);
        ASSERT_NR(rv == GFMRV_OK);
        
        // Update stuff
        while (gfm_isUpdating(pGame->pCtx) == GFMRV_TRUE) {
            rv = gfm_fpsCounterUpdateBegin(pGame->pCtx);
            ASSERT_NR(rv == GFMRV_OK);
            
            // Update everything
            rv = state_mainMenu_update(pGame);
            ASSERT_NR(rv == GFMRV_OK);
            
            rv = gfm_fpsCounterUpdateEnd(pGame->pCtx);
            ASSERT_NR(rv == GFMRV_OK);
        }
        // Draw stuff
        while (gfm_isDrawing(pGame->pCtx) == GFMRV_TRUE) {
            rv = gfm_drawBegin(pGame->pCtx);
            ASSERT_NR(rv == GFMRV_OK);
            
            // Draw everything
            rv = state_mainMenu_draw(pGame);
            ASSERT_NR(rv == GFMRV_OK);
            
            rv = gfm_drawEnd(pGame->pCtx);
            ASSERT_NR(rv == GFMRV_OK);
        }
    }
    
    rv = GFMRV_OK;
__ret:
     state_mainMenu_clean(pGame);
     
    return rv;
}

