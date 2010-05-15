/*
	mars_base - Design, build and admin your own base on Mars
    Copyright (C) 2009  Alfonso Arbona Gimeno (nake90@terra.es). All rights reserved.

	MIT LICENSE
	
	Permission is hereby granted, free of charge, to any
	person obtaining a copy of this software and associated
	documentation files (the "Software"), to deal in the
	Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the
	Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice
	shall be included in all copies or substantial portions of
	the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
	KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
	OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    
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
#define MSG_START       1        /*!< start the dialog, initialise */	// OK
#define MSG_END         2        /*!< dialog is finished - cleanup */	// OK
#define MSG_DRAW        3        /*!< draw the object */				// OK
#define MSG_CLICK       4        /*!< mouse click on the object */
#define MSG_DCLICK      5        /*!< double click on the object */
#define MSG_KEY         6        /*!< keyboard shortcut */
#define MSG_CHAR        7        /*!< other keyboard input */
#define MSG_UCHAR       8        /*!< unicode keyboard input */
#define MSG_XCHAR       9        /*!< broadcast character to all objects */
#define MSG_WANTFOCUS   10       /*!< does object want the input focus? */
#define MSG_GOTFOCUS    11       /*!< got the input focus */
#define MSG_LOSTFOCUS   12       /*!< lost the input focus */
#define MSG_GOTMOUSE    13       /*!< mouse on top of object */			// OK
#define MSG_LOSTMOUSE   14       /*!< mouse moved away from object */	// OK
#define MSG_IDLE        15       /*!< update any background stuff */	// OK
#define MSG_RADIO       16       /*!< clear radio buttons */
#define MSG_WHEEL       17       /*!< mouse wheel moved */
#define MSG_LPRESS      18       /*!< mouse left button pressed */		// OK
#define MSG_LRELEASE    19       /*!< mouse left button released */		// SI
#define MSG_MPRESS      20       /*!< mouse middle button pressed */	// SI
#define MSG_MRELEASE    21       /*!< mouse middle button released */	// SI
#define MSG_RPRESS      22       /*!< mouse right button pressed */		// SI
#define MSG_RRELEASE    23       /*!< mouse right button released */	// SI
#define MSG_WANTMOUSE   24       /*!< does object want the mouse? */
#define MSG_USER        25       /*!< from here on are free... */

#define NULL_DIALOG		NULL, 0, 0, 0, 0 ,{0,0,0,0} ,{0,0,0,0}, 0, 0, 0, 0, NULL, NULL, NULL

#define SPAWN_DLG_MAX_ELEMENTS 256
DIALOG spawn_dialog[SPAWN_DLG_MAX_ELEMENTS];// Ahora es en run-time, pero para no toquitear reallocs y tal hay un máximo de 256 elementos

int do_dialog(DIALOG *d); // Entra en un loop interno y sale con el valor del objeto que ha cerrado el dialog

int draw_dialog(DIALOG *d);
int draw_element(DIALOG d);

/* DIALOG: {(*df), x, y, w, h, fg, bg, key, flag, d1, d2, *dp, *dp2, *dp3} */
int d_box_proc(int msg, struct st_Dialog * d, int c);
int d_line_proc(int msg, struct st_Dialog * d, int c); /* w, h son las coordenadas del segundo punto, d1 el tamaño */
int d_label_proc(int msg, struct st_Dialog * d, int c); /* Texto en *dp , *dp2 la fuente a usar */
int d_image_proc(int msg, struct st_Dialog * d, int c); /* Imagen a usar en d1 */
int d_icon_proc(int msg, struct st_Dialog * d, int c); /* Imagen a usar en d1. Se ilumina el borde y al ser pulsado retorna exit */
int d_button_proc(int msg, struct st_Dialog * d, int c); /* Texto en *dp , *dp2 la fuente a usar, y al ser pulsado retorna exit */


#endif
