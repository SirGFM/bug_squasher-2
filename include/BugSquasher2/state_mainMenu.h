/**
 * @file src/state_mainMenu.c
 * 
 * The game's main menu
 */
#ifndef __STATE_MAINMENU_H__
#define __STATE_MAINMENU_H__

/**
 * The game's main menu
 * 
 * @param  pGame The game "global" context
 * @return       GFMRV_OK, GFMRV_ARGUMENTS_BAD, GFMRV_ALLOC_FAILED, ...
 */
gfmRV state_mainMenu(gameCtx *pGame);

#endif /* __STATE_MAINMENU_H__ */

