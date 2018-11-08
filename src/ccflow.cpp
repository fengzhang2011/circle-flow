/*************************************************
 *
 * Creating the circle-flow.
 *
 * Author: Feng Zhang (zhjinf@gmail.com)
 * Date: 2018-11-08
 * 
 * Copyright:
 *   See LICENSE.
 * 
 ************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <setjmp.h>
#include "libharu/hpdf.h"
#include "libharu/hpdf_utils.h"
#include <iconv.h>
#include <math.h>

#include "ccflow.h"
#include "utils.h"


extern jmp_buf env;

CCFlow::CCFlow() {

  initialize();

}

CCFlow::~CCFlow() {

  finalize();

}

void CCFlow::initialize() {

  pdf = HPDF_New(hpdfErrorHandler, NULL);
  if(!pdf) printf("error: cannot create PdfDoc object\n");

  if(setjmp(env)) finalize();

  // Set to use Chinese encoding.
  HPDF_UseCNSEncodings(pdf);
  HPDF_UseCNSFonts(pdf);
  HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);

}

void CCFlow::finalize() {

  HPDF_Free(pdf);

}

void CCFlow::addPage() {

  HPDF_Point pos;
  const char *detail_font_name;

  /* add a new page object. */
  HPDF_Page page = HPDF_AddPage (pdf);
  // HPDF_Page_Concat (page, 72.0f / 300.0f, 0, 0, 72.0f / 300.0f, 0, 0);

  // Set the page size: A4 by default.
  HPDF_Page_SetHeight (page, 72*11.69);
  HPDF_Page_SetWidth (page, 72*8.27);

  /* draw grid to the page */
  hpdfShowGrid(pdf, page);

  float x = 200;
  float y = 600;
  float radius = 120;

  int N = 5;
  for(int i=0; i<N; i++)
  {
    float angle = (90 - i*360/N)*M_PI/180;
    // printf("angle=%f\n", angle*180/M_PI);
    float nx = radius * cos(angle) + x;
    float ny = radius * sin(angle) + y;
    drawNode(pdf, page, nx, ny, 1.0, 0, 0, "研发部");

    // angle step
    float step = 360/N*M_PI/180;
    float marginFactor = 1.3;
    float margin = marginFactor * 60 * 2 * M_PI / (radius * 2 * M_PI);
    float arcStart = angle - margin/2;
    float arcEnd = angle - step + margin/2;
    // printf("angle=%f step=%f margin=%f radius=%f marginFactor=%f\n", angle*180/M_PI, 360.0/N, margin*180/M_PI, radius, marginFactor);
    //float arcStartX = radius * cos(arcStart) + x;
    //float arcStartY = radius * sin(arcStart) + y;
    //float arcEndX = radius * cos(arcEnd) + x;
    //float arcEndY = radius * sin(arcEnd) + y;
    // printf("arc from %f to %f\n", arcStart*180/M_PI, arcEnd*180/M_PI);

    drawNodeArc(page, x, y, 1.1*radius, 90-1.0*arcStart*180/M_PI, 90-1.0*arcEnd*180/M_PI);
    drawNodeArc(page, x, y, 0.9*radius, 90-1.0*arcEnd*180/M_PI, 90-1.0*arcStart*180/M_PI, false);
  }
  drawNode(pdf, page, x, y, 1.0, 0, 0, "客户");

  drawTable(page);

  drawText(page, 120, 390, 200, 300, 5, 12, "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转。");
  drawText(page, 330, 390, 200, 300, 5, 12, "需要");

  HPDF_Rect rect;
  rect.left = 150;
  rect.top = 350;
  rect.right = 300;
  rect.bottom = 200;
  //HPDF_Destination dst = HPDF_Page_CreateDestination (page);
  //HPDF_Annotation annot = HPDF_Page_CreateLinkAnnot (page, rect, dst);
  //HPDF_LinkAnnot_SetHighlightMode (annot, HPDF_ANNOT_DOWN_APPEARANCE);
  const char* url = "http://www.lm";
  HPDF_Page_CreateURILinkAnnot (page, rect, url);

}

void CCFlow::save(const char* fileName) {

  HPDF_SaveToFile(pdf, fileName);

}

