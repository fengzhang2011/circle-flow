/*************************************************
 *
 * Common utilities for creating the circle-flow.
 *
 * Author: Feng Zhang (zhjinf@gmail.com)
 * Date: 2018-11-08
 * 
 * Copyright:
 *   See LICENSE.
 * 
 * CREDITS:
 *   [1] Print grid on the pdf is based on the Haru demo code grid_sheet.c.
 *       The author is Takeshi Kanno <takeshi_kanno@est.hi-ho.ne.jp>.
 *       URL: https://github.com/libharu/libharu/blob/master/demo/grid_sheet.c
 *   [2] UTF to GBK is based on the code found on lyuting's site.
 *       URL: https://www.jianshu.com/p/cce0cfcd7609
 * 
 ************************************************/

#ifndef _UTILS_INCLUDE_H_
#define _UTILS_INCLUDE_H_


#include "libharu/hpdf.h"


// Error handler used by HPDF.
void hpdfErrorHandler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void* user_data);

// Show the grid on the page.
void hpdfShowGrid(HPDF_Doc pdf, HPDF_Page page);

// Convert UTF-8 coding to GBK.
char *utf2GBK(const char *in_buf);

// Draw a node at (x, y) with color RGB (r, g, b) labeled by 'label'.
void drawNode(HPDF_Doc pdf, HPDF_Page page, float x, float y, float r, float g, float b, const char* label);

// Draw an arc between nodes.
void drawNodeArc(HPDF_Page page, float x, float y, float radius, float start, float end, bool isClockWise = true);

// Determine the number of bytes per character.
int bytesPerCharacter(const char* text);

// Draw text.
void drawText(HPDF_Page page, float x, float y, float width, float height, float margin, float fontSize, const char* text);

#endif // #ifndef _UTILS_INCLUDE_H_
