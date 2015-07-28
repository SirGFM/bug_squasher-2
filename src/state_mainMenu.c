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

/** Main menu struct */
struct stMainMenuCtx {
    /** A tilemap to show the title, in 8x8 tiles */
    gfmTilemap *pTitle;
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
    return GFMRV_OK;
}

/**
 * Draw everything in this state
 * 
 * @param  pGame The game "global" context
 */
static gfmRV state_mainMenu_draw(gameCtx *pGame) {
    return GFMRV_OK;
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

