/*
 * << Haru Free PDF Library 2.0.0 >> -- arc_demo.c
 *
 * Copyright (c) 1999-2006 Takeshi Kanno <takeshi_kanno@est.hi-ho.ne.jp>
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <setjmp.h>
#include "hpdf.h"
#include "hpdf_utils.h"
#include <iconv.h>

jmp_buf env;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler  (HPDF_STATUS   error_no,
                HPDF_STATUS   detail_no,
                void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    longjmp(env, 1);
}


void
print_grid  (HPDF_Doc     pdf,
             HPDF_Page    page)
{
    HPDF_REAL height = HPDF_Page_GetHeight (page);
    HPDF_REAL width = HPDF_Page_GetWidth (page);
    HPDF_Font font = HPDF_GetFont (pdf, "Helvetica", NULL);
    HPDF_UINT x, y;

    HPDF_Page_SetFontAndSize (page, font, 5);
    HPDF_Page_SetGrayFill (page, 0.5);
    HPDF_Page_SetGrayStroke (page, 0.8);

    /* Draw horizontal lines */
    y = 0;
    while (y < height) {
        if (y % 10 == 0)
            HPDF_Page_SetLineWidth (page, 0.5);
        else {
            if (HPDF_Page_GetLineWidth (page) != 0.25)
                HPDF_Page_SetLineWidth (page, 0.25);
        }

        HPDF_Page_MoveTo (page, 0, y);
        HPDF_Page_LineTo (page, width, y);
        HPDF_Page_Stroke (page);

        if (y % 10 == 0 && y > 0) {
            HPDF_Page_SetGrayStroke (page, 0.5);

            HPDF_Page_MoveTo (page, 0, y);
            HPDF_Page_LineTo (page, 5, y);
            HPDF_Page_Stroke (page);

            HPDF_Page_SetGrayStroke (page, 0.8);
        }

        y += 5;
    }


    /* Draw virtical lines */
    x = 0;
    while (x < width) {
        if (x % 10 == 0)
            HPDF_Page_SetLineWidth (page, 0.5);
        else {
            if (HPDF_Page_GetLineWidth (page) != 0.25)
                HPDF_Page_SetLineWidth (page, 0.25);
        }

        HPDF_Page_MoveTo (page, x, 0);
        HPDF_Page_LineTo (page, x, height);
        HPDF_Page_Stroke (page);

        if (x % 50 == 0 && x > 0) {
            HPDF_Page_SetGrayStroke (page, 0.5);

            HPDF_Page_MoveTo (page, x, 0);
            HPDF_Page_LineTo (page, x, 5);
            HPDF_Page_Stroke (page);

            HPDF_Page_MoveTo (page, x, height);
            HPDF_Page_LineTo (page, x, height - 5);
            HPDF_Page_Stroke (page);

            HPDF_Page_SetGrayStroke (page, 0.8);
        }

        x += 5;
    }

    /* Draw horizontal text */
    y = 0;
    while (y < height) {
        if (y % 10 == 0 && y > 0) {
            char buf[12];

            HPDF_Page_BeginText (page);
            HPDF_Page_MoveTextPos (page, 5, y - 2);
#ifdef __WIN32__
            _snprintf (buf, 12, "%u", y);
#else
            snprintf (buf, 12, "%u", y);
#endif
            HPDF_Page_ShowText (page, buf);
            HPDF_Page_EndText (page);
        }

        y += 5;
    }


    /* Draw virtical text */
    x = 0;
    while (x < width) {
        if (x % 50 == 0 && x > 0) {
            char buf[12];

            HPDF_Page_BeginText (page);
            HPDF_Page_MoveTextPos (page, x, 5);
#ifdef __WIN32__
            _snprintf (buf, 12, "%u", x);
#else
            snprintf (buf, 12, "%u", x);
#endif
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

char *utf_2_gbk(char *in_buf, size_t in_len) {
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
    char *in = in_buf;
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

//作者：lyuting
//链接：https://www.jianshu.com/p/cce0cfcd7609
//來源：简书
//简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。

void
draw_line2  (HPDF_Page    page,
             float       x,
             float       y)
{
    HPDF_Page_MoveTo (page, x + 30, y - 25);
    HPDF_Page_LineTo (page, x + 160, y - 25);
    HPDF_Page_Stroke (page);
}

int
main (int argc, char **argv)
{
    HPDF_Doc  pdf;
    HPDF_Page page;
    char fname[256];
    HPDF_Point pos;
    const char *detail_font_name;


    strcpy (fname, argv[0]);
    strcat (fname, ".pdf");

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }

    if (setjmp(env)) {
        HPDF_Free (pdf);
        return 1;
    }

    /* add a new page object. */
    page = HPDF_AddPage (pdf);
    // HPDF_Page_Concat (page, 72.0f / 300.0f, 0, 0, 72.0f / 300.0f, 0, 0);

    /* declaration for using Chinese encoding. */
    HPDF_UseCNSEncodings (pdf);
    HPDF_UseCNSFonts (pdf);
    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

    detail_font_name = HPDF_LoadTTFontFromFile (pdf, "/storage/feng/code/circle-flow/fonts/SourceHanSansCN-Light.ttf", HPDF_TRUE);
//    printf("detail_font_name = %s\n", detail_font_name);

    HPDF_Page_SetHeight (page, 72*11.69);
    HPDF_Page_SetWidth (page, 72*8.27);

    /* draw grid to the page */
    print_grid  (pdf, page);

    HPDF_Page_SetGrayStroke (page, 0);
    HPDF_Page_SetRGBFill (page, 0, 0, 1.0);
    HPDF_Page_Circle (page, 100, 100, 30);
    HPDF_Page_Fill (page);

    HPDF_Font detail_font = HPDF_GetFont (pdf, detail_font_name, "GBK-EUC-H");
    //HPDF_Font detail_font = HPDF_GetFont (pdf, "SimHei", "GBK-EUC-H");

    HPDF_Page_BeginText (page);

    HPDF_Page_SetRGBFill (page, 1.0, 1.0, 0);
//    HPDF_Page_ShowText (page, detail_font_name);
//    HPDF_Page_ShowText (page, " (");
//    HPDF_Page_ShowText (page, HPDF_Font_GetEncodingName (detail_font));
//    HPDF_Page_ShowText (page, "中国)");

//    HPDF_UseCNSFonts(pdf);
//    HPDF_UseCNSEncodings(pdf);
//    HPDF_Font font = HPDF_GetFont(pdf, "SimSun", "GB-EUC-H");

    char text[] = "乱七八糟";
    char* text2 = utf_2_gbk(text, strlen(text));

    HPDF_Page_SetFontAndSize (page, detail_font, 12);
    float textWidth = HPDF_Page_TextWidth(page, text2);
    printf("textWidth = %f\n", textWidth);

    /* move the position of the text to top of the page. */
    HPDF_Page_MoveTextPos(page, 100-textWidth/2, 100-12/2);

//    HPDF_Page_BeginText (page);
    HPDF_Page_ShowText (page, text2);
    HPDF_Page_EndText (page);

    /* finish to print text. */
    // HPDF_Page_EndText (page);

    HPDF_Page_SetLineWidth (page, 2);
    HPDF_Page_SetLineCap (page, HPDF_PROJECTING_SCUARE_END);
    draw_line2 (page, 60, 440);

    HPDF_Page_SetLineJoin (page, HPDF_ROUND_JOIN);
    HPDF_Page_MoveTo (page, 120, 195);
    HPDF_Page_LineTo (page, 160, 235);
    HPDF_Page_LineTo (page, 200, 195);
    HPDF_Page_Stroke (page);

    HPDF_Page_Arc (page, 100, 100, 80, 360 * 0.75, 360);
    pos = HPDF_Page_GetCurrentPos (page);
    HPDF_Page_LineTo (page, pos.x-10, pos.y+10);
    HPDF_Page_MoveTo (page, pos.x, pos.y);
    HPDF_Page_LineTo (page, pos.x-10, pos.y-10);
    HPDF_Page_Stroke (page);
    /* draw pie chart
     *
     *   A: 45% Red
     *   B: 25% Blue
     *   C: 15% green
     *   D: other yellow
     */

    /* A */
//    HPDF_Page_SetRGBFill (page, 1.0, 0, 0);
//    HPDF_Page_MoveTo (page, 100, 100);
//    HPDF_Page_LineTo (page, 100, 180);
//    HPDF_Page_Arc (page, 100, 100, 80, 0, 360 * 0.45);
//    pos = HPDF_Page_GetCurrentPos (page);
//    HPDF_Page_LineTo (page, 100, 100);
//    HPDF_Page_Fill (page);

    /* B */
//    HPDF_Page_SetRGBFill (page, 0, 0, 1.0);
//    HPDF_Page_MoveTo (page, 100, 100);
//    HPDF_Page_LineTo (page, pos.x, pos.y);
//    HPDF_Page_Arc (page, 100, 100, 80, 360 * 0.45, 360 * 0.7);
//    pos = HPDF_Page_GetCurrentPos (page);
//    HPDF_Page_LineTo (page, 100, 100);
//    HPDF_Page_Fill (page);
//
//    /* C */
//    HPDF_Page_SetRGBFill (page, 0, 1.0, 0);
//    HPDF_Page_MoveTo (page, 100, 100);
//    HPDF_Page_LineTo (page, pos.x, pos.y);
//    HPDF_Page_Arc (page, 100, 100, 80, 360 * 0.7, 360 * 0.85);
//    pos = HPDF_Page_GetCurrentPos (page);
//    HPDF_Page_LineTo (page, 100, 100);
//    HPDF_Page_Fill (page);
//
//    /* D */
//    HPDF_Page_SetRGBFill (page, 1.0, 1.0, 0);
//    HPDF_Page_MoveTo (page, 100, 100);
//    HPDF_Page_LineTo (page, pos.x, pos.y);
//    HPDF_Page_Arc (page, 100, 100, 80, 360 * 0.85, 360);
//    pos = HPDF_Page_GetCurrentPos (page);
//    HPDF_Page_LineTo (page, 100, 100);
//    HPDF_Page_Fill (page);
//
//    /* draw center circle */
//    HPDF_Page_SetGrayStroke (page, 0);
//    HPDF_Page_SetGrayFill (page, 1);
//    HPDF_Page_Circle (page, 100, 100, 30);
//    HPDF_Page_Fill (page);

    /* save the document to a file */
    HPDF_SaveToFile (pdf, fname);

    /* clean up */
    HPDF_Free (pdf);

    return 0;
}

