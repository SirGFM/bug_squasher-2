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
#include <GFraMe/gfmText.h>
#include <GFraMe/gfmTilemap.h>

#include <stdlib.h>
#include <string.h>

/** Title data for the tilemap */
int titleData[] = {
#include "../assets/title.csv"
};
/** Background data for the tilemap */
int backgroundData[] = {
#include "../assets/background.csv"
};
/** Floor data for the tilemap */
int floorData[] = {
#include "../assets/floor.csv"
};

/** Main menu struct */
struct stMainMenuCtx {
    /** "touch to play" text */
    gfmText *pText;
    /** A tilemap to show the title, in 8x8 tiles */
    gfmTilemap *pTitle;
    /** A tilemap to show the background, in 8x8 tiles */
    gfmTilemap *pBackground;
    /** A tilemap to show the floor, in 16x tiles */
    gfmTilemap *pFloor;
    /** Current position in the "title animation" (copying of the tilemap) */
    int titleAnimation;
    /** Accumulator for the animation */
    int animationTime;
    /** How many frames (at 60 fps) the animation takes */
    int numAnimFrames;
    /** How many frames have passed */
    int frameCount;
    /** Text to display or hide the text */
    int textTimer;
};

/**
 * Initialize everything needed by the main menu
 * 
 * @param  pGame The game "global" context
 * @return       GFMRV_OK, GFMRV_ARGUMENTS_BAD, GFMRV_ALLOC_FAILED, ...
 */
static gfmRV state_mainMenu_init(gameCtx *pGame) {
    gfmRV rv;
    int i;
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
    
    // Alloc and load the floor
    rv = gfmTilemap_getNew(&(pMainMenu->pFloor));
    ASSERT_NR(rv == GFMRV_OK);
    rv = gfmTilemap_init(pMainMenu->pFloor, pGame->pSset16x16, 20, 8, -1);
    ASSERT_NR(rv == GFMRV_OK);
    rv = gfmTilemap_loadStatic(pMainMenu->pFloor, floorData, 20, 4);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Alloc and initialize the "touch to play" text
    rv = gfmText_getNew(&(pMainMenu->pText));
    ASSERT_NR(rv == GFMRV_OK);
    rv = gfmText_init(pMainMenu->pText, 96/*x*/, 168/*y*/, 17/*maxWidth*/,
            1/*maxLines*/, 0/*delay*/, 0/*doBindToScreen*/, pGame->pSset8x8,
            0/*firstTile*/);
    ASSERT_NR(rv == GFMRV_OK);
    // TODO Initialize accordingly with desktop or mobile
    rv = gfmText_setTextStatic(pMainMenu->pText, "--TOUCH TO PLAY--", 1);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Count how many frames there will be in the animation
    pMainMenu->numAnimFrames = 0;
    i = 0;
    while (i < sizeof(titleData) / sizeof(int)) {
        if (titleData[i] != -1) {
            pMainMenu->numAnimFrames++;
        }
        i++;
    }
    
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
    // Clean the floor
    gfmTilemap_free(&(pGame->pMainMenu->pFloor));
    // Clean the text
    gfmText_free(&(pGame->pMainMenu->pText));
    
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
    int camWidth, camHeight, elapsed, y;
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
            pData[pMainMenu->titleAnimation] =
                    titleData[pMainMenu->titleAnimation];
            
            pMainMenu->titleAnimation++;
            pMainMenu->frameCount++;
        }
        pMainMenu->animationTime -= 16;
    }
    
    // Get the game's dimensions
    rv = gfm_getCameraDimensions(&camWidth, &camHeight, pGame->pCtx);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Animate the BG and floor position
    // BG position is calculate as "-height + (height + pos) * alpha" to start
    // it outside the screen and move until 'pos'
    y = -128 + (128 + 56) * pMainMenu->frameCount / pMainMenu->numAnimFrames;
    rv = gfmTilemap_setPosition(pMainMenu->pBackground, 0, y);
    ASSERT_NR(rv == GFMRV_OK);
    // For the same reason, the floor position is calculated as
    // "WND_HEIGHT - (WND_HEIGHT - pos) * alpha"
    y = camHeight - (camHeight - 176) * pMainMenu->frameCount /
            pMainMenu->numAnimFrames;
    rv = gfmTilemap_setPosition(pMainMenu->pFloor, 0, y);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Update the text timer (it will only render on positive times, and should
    // flick every half second)
    pMainMenu->textTimer += elapsed;
    if (pMainMenu->textTimer >= 500) {
        pMainMenu->textTimer -= 1000;
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
    // Draw the floor
    rv = gfmTilemap_draw(pMainMenu->pFloor, pGame->pCtx);
    ASSERT_NR(rv == GFMRV_OK);
    // Draw the title
    rv = gfmTilemap_draw(pMainMenu->pTitle, pGame->pCtx);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Draw the text
    if (pMainMenu->textTimer >= 0) {
        rv = gfmText_draw(pMainMenu->pText, pGame->pCtx);
        ASSERT_NR(rv == GFMRV_OK);
    }
    
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

