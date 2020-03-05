#ifndef ATLAS_H
#define ATLAS_H

#include "microui.h"

/* this head file provides declarations of the altas data
 * and functions defined in atlas.c.
 */

enum { ATLAS_WHITE = MU_ICON_MAX, ATLAS_FONT };
enum { ATLAS_WIDTH = 128, ATLAS_HEIGHT = 128 };

extern const unsigned char atlas_texture[ATLAS_WIDTH * ATLAS_HEIGHT];
extern const mu_Rect atlas[];

int atlas_text_width(mu_Font font, const char *text, int len);
int atlas_text_height(mu_Font font);

#endif
