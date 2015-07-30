/**
 * @file src/main.c
 * 
 * Game's entry point; Spawns a thread for loading all the assets and then
 * starts the game
 */
#include <BugSquasher2/gameCtx.h>
#include <BugSquasher2/state_mainMenu.h>

#include <GFraMe/gframe.h>
#include <GFraMe/gfmAssert.h>
#include <GFraMe/gfmError.h>
#include <GFraMe/gfmInput.h>
#include <GFraMe/gfmSpriteset.h>

#include <string.h>

/** The game virtual window's width */
#define VWIDTH     320
/** The game virtual window's height */
#define VHEIGHT    240
/** The game default window's width */
#define WND_WIDTH  640
/** The game default window's height */
#define WND_HEIGHT 480
/** The game's main (and only) texture */
#define TEXTURE    "atlas.bmp"
/** Transparent color */
#define COLOR_KEY  0xff00ff
/** Basic FPS */
#define FPS        60

/**
 * Loads all game's assets
 * 
 * @param  pGame The game context
 * @return       GFMRV_OK, GFMRV_ARGUMENTS_BAD, ...
 */
static gfmRV loadAssets(gameCtx *pGame) {
    gfmRV rv;
    
    // Sanitize arguments
    ASSERT(pGame, GFMRV_ARGUMENTS_BAD);
    
    // Load the texture
    rv = gfm_loadTextureStatic(&(pGame->iTex), pGame->pCtx, TEXTURE, COLOR_KEY);
    ASSERT_NR(rv == GFMRV_OK);
    // Set it as the default
    rv = gfm_setDefaultTexture(pGame->pCtx, pGame->iTex);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Create the spritesets
    rv = gfm_createSpritesetCached(&(pGame->pSset8x8), pGame->pCtx, pGame->iTex,
            8/*tw*/, 8/*th*/);
    ASSERT_NR(rv == GFMRV_OK);
    rv = gfm_createSpritesetCached(&(pGame->pSset8x16), pGame->pCtx,
            pGame->iTex, 8/*tw*/, 16/*th*/);
    ASSERT_NR(rv == GFMRV_OK);
    rv = gfm_createSpritesetCached(&(pGame->pSset16x16), pGame->pCtx,
            pGame->iTex, 16/*tw*/, 16/*th*/);
    ASSERT_NR(rv == GFMRV_OK);
    rv = gfm_createSpritesetCached(&(pGame->pSset32x32), pGame->pCtx,
            pGame->iTex, 32/*tw*/, 32/*th*/);
    ASSERT_NR(rv == GFMRV_OK);
    
    rv = GFMRV_OK;
__ret:
    return rv;
}

/**
 * Game entry point
 * 
 * @param  argc Number of args
 * @param  argv List of args
 */
int main(int argc, char *argv[]) {
    gameCtx game;
    gfmRV rv;
    
    // Set all default values
    memset(&game, 0x0, sizeof(gameCtx));
    
    // Alloc and initialize the library
    rv = gfm_getNew(&(game.pCtx));
    ASSERT_NR(rv == GFMRV_OK);
    rv = gfm_init(game.pCtx);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Set the game's title
    rv = gfm_setTitleStatic(game.pCtx, "com.gfmgamecorner", "BugSquasher2");
    ASSERT_NR(rv == GFMRV_OK);
    
    // Initialize the window (last param is whether the user can resize the wnd)
    rv = gfm_initGameWindow(game.pCtx, VWIDTH, VHEIGHT, WND_WIDTH, WND_HEIGHT,
            0);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Load all assets
    // TODO push this into another thread and play an animation
    rv = loadAssets(&game);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Add all virtual keys
    // Add action (i.e., jump & select) key
    rv = gfm_addVirtualKey(&(game.actionHnd), game.pCtx);
    ASSERT_NR(rv == GFMRV_OK);
    // TODO Add missing keys
    
    // TODO Customize the inputs
    rv = gfm_bindInput(game.pCtx, game.actionHnd, gfmPointer_button);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Initalize the FPS counter (will only work on DEBUG mode)
    rv = gfm_initFPSCounter(game.pCtx, game.pSset8x8, 0/*firstTile*/);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Set the main loop framerate
    rv = gfm_setStateFrameRate(game.pCtx, FPS, FPS);
    ASSERT_NR(rv == GFMRV_OK);
    // Initialize the timer
    rv = gfm_setFPS(game.pCtx, FPS);
    ASSERT_NR(rv == GFMRV_OK);
    
    // Run until the window is closed
    game.isRunning = 1;
    game.state = game_mainMenu;
    while (game.isRunning && gfm_didGetQuitFlag(game.pCtx) == GFMRV_FALSE) {
        // TODO Remove this! (only here to be able to run the game until any
        // state is done)
        rv = gfm_handleEvents(game.pCtx);
        ASSERT_NR(rv == GFMRV_OK);
        
        switch(game.state) {
            case game_mainMenu: rv = state_mainMenu(&game); break;
            default: rv = GFMRV_INTERNAL_ERROR;
        }
        ASSERT_NR(rv == GFMRV_OK);
        
        // Clear the switch flag
        game.switchState = 0;
    }
    
    rv = GFMRV_OK;
__ret:
    // Make sure everything is cleaned, even on error
    gfm_free(&(game.pCtx));
    
    return rv;
}

