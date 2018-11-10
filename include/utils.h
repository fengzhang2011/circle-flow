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


// Colors
typedef struct { float r; float g; float b; } color_t;

const static color_t color_red = { .r=0.9, .g=0.1, .b=0.29 };
const static color_t color_green = { .r=0.24, .g=0.71, .b=0.29 };
const static color_t color_blue = { .r=0.26, .g=0.39, .b=0.85 };
const static color_t color_yellow = { .r=1.0, .g=0.88, .b=0.1 };
const static color_t color_orange = { .r=0.96, .g=0.51, .b=0.19 };
const static color_t color_purple = { .r=0.57, .g=0.12, .b=0.71 };
const static color_t color_cyan = { .r=0.26, .g=0.83, .b=0.96 };
const static color_t color_magenta = { .r=0.94, .g=0.20, .b=0.90 };
const static color_t color_lime = { .r=0.75, .g=0.94, .b=0.27 };
const static color_t color_pink = { .r=0.98, .g=0.75, .b=0.75 };
const static color_t color_teal = { .r=0.27, .g=0.6, .b=0.56 };
const static color_t color_lavender = { .r=0.90, .g=0.75, .b=1.0 };
const static color_t color_brown = { .r=0.6, .g=0.39, .b=0.14 };
const static color_t color_beige = { .r=1.0, .g=0.98, .b=0.78 };
const static color_t color_maroon = { .r=0.5, .g=0.0, .b=0.0 };
const static color_t color_mint = { .r=0.67, .g=1.0, .b=0.76 };
const static color_t color_olive = { .r=0.5, .g=0.5, .b=0.0 };
const static color_t color_apricot = { .r=1.0, .g=0.85, .b=0.69 };
const static color_t color_navy = { .r=0.0, .g=0.0, .b=0.46 };
const static color_t color_aqua = { .r=0.5, .g=0.86, .b=1.0 };
const static color_t color_fuchsia = { .r=0.94, .g=0.07, .b=0.75 };
const static color_t color_grey = { .r=0.66, .g=0.66, .b=0.66 };
const static color_t color_silver = { .r=0.87, .g=0.87, .b=0.87 };
const static color_t color_white = { .r=1.0, .g=1.0, .b=1.0 };
const static color_t color_black = { .r=0.0, .g=0.0, .b=0.0 };

// Colors for the circular diagram. REFERENCE: http://erkal.jonathandedecker.com/circle-flow-chart/
// cdn stands for circular diagram node
const static color_t color_cfn_center = { .r=0.91, .g=0.17, .b=0.2 }; // #E72C33
const static color_t color_cfn_1 = { .r=0.97, .g=0.55, .b=0.28 };// #F78B48
const static color_t color_cfn_2 = { .r=0.98, .g=0.76, .b=0.33 };// #FBC255
const static color_t color_cfn_3 = { .r=0.53, .g=0.73, .b=0.23 };// #88BB3A
const static color_t color_cfn_4 = { .r=0.24, .g=0.70, .b=0.29 };// #3DB34B
const static color_t color_cfn_5 = { .r=0.28, .g=0.73, .b=0.98 };// #48BBFB
const static color_t color_cfn_6 = { .r=0.12, .g=0.58, .b=0.95 };// #1F94F1
const static color_t color_cfn_7 = { .r=0.62, .g=0.43, .b=0.78 };// #9E6EC7
const static color_t color_cfn_8 = { .r=0.75, .g=0.51, .b=0.28 };// #C08248



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
