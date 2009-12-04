/*
	mars_base - Design, build and maintain your own base on Mars
    Copyright (C) 2009  Alfonso Arbona Gimeno (nake90@terra.es). All rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    If you use any part of this code you must give me (Alfonso Arbona Gimeno) credit.
    If you plan to use any part of this code on a comercial game please email me at:
	   	   nake90@terra.es
*/

/** \file overlay.h
 * \brief Header que controla el HUD
 * \author Alfonso Arbona Gimeno
*/

#ifndef OVERLAY_H
#define OVERLAY_H
#include "shared.h"


void draw_minimap(GLuint minimap);
void draw_HUD(void);

/* - BASADO EN ALLEGRO - Muchas cosas posiblemente omitidas... */

typedef struct st_Dialog
{
	int (*df)(int msg, struct st_Dialog * d, int c);
	int x, y;
	int w, h;
	COLORf fg, bg;
	int key;
	int flag;
	int d1, d2;
	void *dp, *dp2, *dp3;
}DIALOG;

/* bits for the flags field */
#define D_EXIT          1        /*!< object makes the dialog exit */
#define D_SELECTED      2        /*!< object is selected */
#define D_GOTFOCUS      4        /*!< object has the input focus */
#define D_GOTMOUSE      8        /*!< mouse is on top of object */
#define D_HIDDEN        16       /*!< object is not visible */
#define D_DISABLED      32       /*!< object is visible but inactive */
#define D_DIRTY         64       /*!< object needs to be redrawn */
#define D_INTERNAL      128      /*!< reserved for internal use */
#define D_USER          256      /*!< from here on is free for your own use */


/* return values for the dialog procedures */
#define D_O_K           0        /*!< normal exit status */
#define D_CLOSE         1        /*!< request to close the dialog */
#define D_REDRAW        2        /*!< request to redraw the dialog */
#define D_REDRAWME      4        /*!< request to redraw this object */
#define D_WANTFOCUS     8        /*!< this object wants the input focus */
#define D_USED_CHAR     16       /*!< object has used the keypress */
#define D_REDRAW_ALL    32       /*!< request to redraw all active dialogs */
#define D_DONTWANTMOUSE 64       /*!< this object does not want mouse focus */


/* messages for the dialog procedures */
#define MSG_START       1        /*!< start the dialog, initialise */
#define MSG_END         2        /*!< dialog is finished - cleanup */
#define MSG_DRAW        3        /*!< draw the object */
#define MSG_CLICK       4        /*!< mouse click on the object */
#define MSG_DCLICK      5        /*!< double click on the object */
#define MSG_KEY         6        /*!< keyboard shortcut */
#define MSG_CHAR        7        /*!< other keyboard input */
#define MSG_UCHAR       8        /*!< unicode keyboard input */
#define MSG_XCHAR       9        /*!< broadcast character to all objects */
#define MSG_WANTFOCUS   10       /*!< does object want the input focus? */
#define MSG_GOTFOCUS    11       /*!< got the input focus */
#define MSG_LOSTFOCUS   12       /*!< lost the input focus */
#define MSG_GOTMOUSE    13       /*!< mouse on top of object */
#define MSG_LOSTMOUSE   14       /*!< mouse moved away from object */
#define MSG_IDLE        15       /*!< update any background stuff */
#define MSG_RADIO       16       /*!< clear radio buttons */
#define MSG_WHEEL       17       /*!< mouse wheel moved */
#define MSG_LPRESS      18       /*!< mouse left button pressed */
#define MSG_LRELEASE    19       /*!< mouse left button released */
#define MSG_MPRESS      20       /*!< mouse middle button pressed */
#define MSG_MRELEASE    21       /*!< mouse middle button released */
#define MSG_RPRESS      22       /*!< mouse right button pressed */
#define MSG_RRELEASE    23       /*!< mouse right button released */
#define MSG_WANTMOUSE   24       /*!< does object want the mouse? */
#define MSG_USER        25       /*!< from here on are free... */

int draw_element(DIALOG d);

int d_box_proc(int msg, struct st_Dialog * d, int c);
int d_line_proc(int msg, struct st_Dialog * d, int c); /* w, h son las coordenadas del segundo punto, d1 el tamaño */
int d_label_proc(int msg, struct st_Dialog * d, int c); /* Texto en *dp , d1 el tamaño (de 1 a 16) */


#endif
