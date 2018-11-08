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

#include <iconv.h>
#include <math.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

jmp_buf env;

// Error handler used by HPDF.
void hpdfErrorHandler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void* user_data) {

  printf("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no, (HPDF_UINT)detail_no);
  longjmp(env, 1);

}

// Show the grid on the page.
void hpdfShowGrid(HPDF_Doc pdf, HPDF_Page page)
{
  HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
  HPDF_Page_SetFontAndSize (page, font, 5);
  HPDF_Page_SetGrayFill (page, 0.5);
  HPDF_Page_SetGrayStroke (page, 0.8);

  HPDF_REAL width = HPDF_Page_GetWidth(page);
  HPDF_REAL height = HPDF_Page_GetHeight(page);

  char buf[12];
  HPDF_UINT x, y;
  // Draw horizontal lines and text.
  y = 0;
  while(y < height) {
    if(y % 10 == 0)
      HPDF_Page_SetLineWidth(page, 0.5);
    else if(HPDF_Page_GetLineWidth(page) != 0.25)
      HPDF_Page_SetLineWidth(page, 0.25);
    HPDF_Page_MoveTo(page, 0, y);
    HPDF_Page_LineTo(page, width, y);
    HPDF_Page_Stroke(page);
    if(y % 10 == 0 && y > 0) {
      HPDF_Page_SetGrayStroke(page, 0.5);
      HPDF_Page_MoveTo(page, 0, y);
      HPDF_Page_LineTo(page, 5, y);
      HPDF_Page_Stroke(page);
      HPDF_Page_SetGrayStroke(page, 0.8);
      // Draw texts.
      HPDF_Page_BeginText(page);
      HPDF_Page_MoveTextPos(page, 5, y - 2);
      snprintf(buf, 12, "%u", y);
      HPDF_Page_ShowText(page, buf);
      HPDF_Page_EndText(page);
    }
    y += 5;
  }

  // Draw vertical lines.
  x = 0;
  while(x < width) {
    if(x % 10 == 0)
      HPDF_Page_SetLineWidth(page, 0.5);
    else if(HPDF_Page_GetLineWidth(page) != 0.25)
      HPDF_Page_SetLineWidth(page, 0.25);
    HPDF_Page_MoveTo(page, x, 0);
    HPDF_Page_LineTo(page, x, height);
    HPDF_Page_Stroke(page);
    if(x % 50 == 0 && x > 0) {
      HPDF_Page_SetGrayStroke (page, 0.5);
      HPDF_Page_MoveTo (page, x, 0);
      HPDF_Page_LineTo (page, x, 5);
      HPDF_Page_Stroke (page);
      HPDF_Page_MoveTo (page, x, height);
      HPDF_Page_LineTo (page, x, height - 5);
      HPDF_Page_Stroke (page);
      HPDF_Page_SetGrayStroke (page, 0.8);
      // Draw texts.
      HPDF_Page_BeginText (page);
      HPDF_Page_MoveTextPos (page, x, 5);
      snprintf (buf, 12, "%u", x);
      HPDF_Page_ShowText (page, buf);
      HPDF_Page_EndText (page);
      HPDF_Page_BeginText (page);
      HPDF_Page_MoveTextPos (page, x, height - 10);
      HPDF_Page_ShowText (page, buf);
      HPDF_Page_EndText (page);
    }
    x += 5;
  }

  HPDF_Page_SetGrayFill (page, 0);
  HPDF_Page_SetGrayStroke (page, 0);
}

// Convert UTF-8 coding to GBK.
char *utf2GBK(const char *in_buf)
{
  size_t in_len = strlen(in_buf);
  iconv_t cd = iconv_open("GBK", "UTF-8");
  if (cd == (iconv_t)-1) {
    perror("获取字符转换描述符失败！\n");
    return NULL;
  }
  size_t sz = in_len * in_len;
  char *tmp_str = (char *)malloc(sz);
  // 不要将原始的指针传进去，那样会改变原始指针的
  size_t inlen = in_len;
  size_t outlen = sz;
  char *in = (char*)in_buf;
  char *out = tmp_str;
  if (tmp_str == NULL) {
    iconv_close(cd);
    fprintf(stderr, "分配内存失败！\n");
    return NULL;
  }
  memset(tmp_str, 0, sz);
  if (iconv(cd, &in, &inlen, &out, &outlen) == (size_t)-1) {
    iconv_close(cd);
    return NULL;
  }
  iconv_close(cd);
  return tmp_str;
}

// Draw a node at (x, y) with color RGB (r, g, b) labeled by 'label'.
void drawNode(HPDF_Doc pdf, HPDF_Page page, float x, float y, float r, float g, float b, const char* label)
{
  HPDF_Page_SetGrayStroke (page, 0);
  HPDF_Page_SetRGBFill (page, 0, 0, 1.0);
  HPDF_Page_Circle (page, x, y, 30);
  HPDF_Page_Fill (page);

  HPDF_Page_BeginText (page);

  HPDF_Page_SetRGBFill (page, 1.0, 0, 0);
  char* text = utf2GBK(label);
  float textWidth = HPDF_Page_TextWidth(page, text);

  /* move the position of the text to top of the page. */
  HPDF_Page_MoveTextPos(page, x-textWidth/2, y-12/2);

//  HPDF_Page_BeginText (page);
  HPDF_Page_ShowText (page, text);
  HPDF_Page_EndText (page);

  free(text);
  text = NULL;
}

// Draw an arc between nodes.
void drawNodeArc(HPDF_Page page, float x, float y, float radius, float start, float end, bool isClockWise)
{
  HPDF_Page_SetLineWidth (page, 2);
  HPDF_Page_SetLineJoin (page, HPDF_ROUND_JOIN);

  // printf("arc %f ==> %f\n", start, end);

  HPDF_Page_Arc (page, x, y, radius, start, end);
  HPDF_Point pos = HPDF_Page_GetCurrentPos (page);
  float x1, y1, x2, y2;
  if(isClockWise) {
    x1 = pos.x + 10 * cos((180-end+25)*M_PI/180);
    y1 = pos.y + 10 * sin((180-end+25)*M_PI/180);
    x2 = pos.x + 10 * cos((180-end-20)*M_PI/180);
    y2 = pos.y + 10 * sin((180-end-20)*M_PI/180);
  } else {
    x1 = pos.x + 10 * cos((-end+20)*M_PI/180);
    y1 = pos.y + 10 * sin((-end+20)*M_PI/180);
    x2 = pos.x + 10 * cos((-end-25)*M_PI/180);
    y2 = pos.y + 10 * sin((-end-25)*M_PI/180);
  }
  HPDF_Page_LineTo (page, x1, y1);
  HPDF_Page_MoveTo (page, pos.x, pos.y);
  HPDF_Page_LineTo (page, x2, y2);
  HPDF_Page_Stroke (page);
}

// Determine the number of bytes per character.
int bytesPerCharacter(const char* text)
{
  int firstByte = (int) *text & 0xFF;

  if( (firstByte & 0xF8) == 0xF0) return 4;
  else if( (firstByte & 0xF0) == 0xE0 ) return 3;
  else if( (firstByte & 0xE0) == 0xC0 ) return 2;
  else if( (firstByte & 0x80) == 0 ) return 1;

  return 0;
}

// Draw text.
void drawText(HPDF_Page page, float x, float y, float width, float height, float margin, float fontSize, const char* text)
{
  char* content = utf2GBK(text);
  float textWidth = HPDF_Page_TextWidth(page, content);

  HPDF_Page_BeginText(page);
  // printf("text_width=%f\n", textWidth);
  char* p = (char*)text;
  char test[5] = {'\0'};
  int count = 0;
  HPDF_Page_MoveTextPos(page, x+margin, y-margin-fontSize);
  while(*p!='\0') {
    int nbBytesPerChar = bytesPerCharacter(p);
    int j = 0;
    memset(test, 0, 5);
    memcpy(test, p, nbBytesPerChar);
    HPDF_Page_ShowText(page, utf2GBK(test));
    p += nbBytesPerChar;
    // printf("%s\n", test);
    count ++;
    if(fontSize*count > width-2*margin) {
      HPDF_Page_MoveTextPos(page, 0, -fontSize-fontSize*0.2);
      // printf("start a new line\n");
      count = 0;
    }
  }
  HPDF_Page_EndText(page);
  return;
}

