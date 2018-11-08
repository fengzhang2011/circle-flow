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

  // const char* font_name = HPDF_LoadTTFontFromFile(pdf, "../fonts/SourceHanSansCN-Light.ttf", HPDF_TRUE);
  // font = HPDF_GetFont(pdf, font_name, "GBK-EUC-H");
  font = HPDF_GetFont(pdf, "SimHei", "GBK-EUC-H");

}

void CCFlow::finalize() {

  HPDF_Free(pdf);

}

HPDF_Page CCFlow::addPage(std::string pageId) {

  HPDF_Page page = HPDF_AddPage(pdf);

  // Change the DPI. (The following statement changes 72dpi to 300 dpi.)
  // HPDF_Page_Concat (page, 72.0f / 300.0f, 0, 0, 72.0f / 300.0f, 0, 0);

  // Set the page size: A4 by default.
  HPDF_Page_SetHeight (page, 72*11.69);
  HPDF_Page_SetWidth (page, 72*8.27);

  // Draw grid to the page.
  hpdfShowGrid(pdf, page);

  // Set font and size.
  HPDF_Page_SetFontAndSize(page, font, 12);

  // Insert to pages map.
  pages.insert(std::pair<std::string, HPDF_Page>(pageId, page));

  return page;

}

void CCFlow::drawCircle(HPDF_Page page, float x, float y, float radius, std::string center, std::vector<std::string> nodes) {

  int N = nodes.size();

  for(int i=0; i<N; i++)
  {
    float angle = (90 - i*360/N)*M_PI/180;
    // printf("angle=%f\n", angle*180/M_PI);
    float nx = radius * cos(angle) + x;
    float ny = radius * sin(angle) + y;
    drawNode(pdf, page, nx, ny, 1.0, 0, 0, nodes[i].c_str());

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

  drawNode(pdf, page, x, y, 1.0, 0, 0, center.c_str());

}

// Draw table.
void drawTable(HPDF_Page page, float left, float top, float bottom, float right)
{
  HPDF_Page_SetLineWidth(page, 1);
  HPDF_Rect rect;
  rect.left = left;
  rect.top = top;
  rect.bottom = bottom;
  rect.right = right;

  HPDF_Page_Rectangle(page, rect.left, rect.bottom, rect.right - rect.left,
                rect.top - rect.bottom);

  HPDF_Page_MoveTo(page, left, top-25);
  HPDF_Page_LineTo(page, right, top-25);

  HPDF_Page_MoveTo(page, left+60, bottom);
  HPDF_Page_LineTo(page, left+60, top-25);
  HPDF_Page_MoveTo(page, left+285, bottom);
  HPDF_Page_LineTo(page, left+285, top-25);

  HPDF_Page_MoveTo(page, left, top-50);
  HPDF_Page_LineTo(page, right, top-50);

  HPDF_Page_Stroke(page);
}

void addLink(HPDF_Page page) {

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

void addResponsibilities(HPDF_Page page) {

  drawTable(page, 50, 440, 40, 550);

  // Add the supervisor.
  const char* supervisor = "总负责人： XXX";
  const char* date = "日期： 2018年11月09日";
  drawText(page, 50, 440, 200, 50, 5, 12, supervisor);
  drawText(page, 415, 440, 200, 50, 5, 12, date);
  
  // header
  drawText(page, 50, 415, 70, 300, 5, 12, "责任人");
  drawText(page, 110, 415, 200, 300, 5, 12, "输入");
  drawText(page, 335, 415, 200, 300, 5, 12, "输出");

  // first role
  drawText(page, 50, 390, 70, 300, 5, 12, "研发部");
  drawText(page, 50, 370, 70, 300, 5, 12, "张三");
  drawText(page, 110, 390, 200, 300, 5, 12, "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转");
  drawText(page, 335, 390, 200, 300, 5, 12, "需要");

  // second role
  drawText(page, 50, 335, 70, 300, 5, 12, "研发部");
  drawText(page, 50, 315, 70, 300, 5, 12, "李四");
  drawText(page, 110, 335, 200, 300, 5, 12, "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转");
  drawText(page, 335, 335, 200, 300, 5, 12, "需要");

  // third role
  drawText(page, 50, 280, 70, 300, 5, 12, "研发部");
  drawText(page, 50, 260, 70, 300, 5, 12, "李四");
  drawText(page, 110, 280, 200, 300, 5, 12, "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转");
  drawText(page, 335, 280, 200, 300, 5, 12, "需要");

  // forth role
  drawText(page, 50, 225, 70, 300, 5, 12, "研发部");
  drawText(page, 50, 205, 70, 300, 5, 12, "李四");
  drawText(page, 110, 225, 200, 300, 5, 12, "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转");
  drawText(page, 335, 225, 200, 300, 5, 12, "需要");

  // fifth role
  drawText(page, 50, 170, 70, 300, 5, 12, "研发部");
  drawText(page, 50, 150, 70, 300, 5, 12, "李四");
  drawText(page, 110, 170, 200, 300, 5, 12, "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转");
  drawText(page, 335, 170, 200, 300, 5, 12, "需要");

  // sixth role
  drawText(page, 50, 115, 70, 300, 5, 12, "研发部");
  drawText(page, 50, 95, 70, 300, 5, 12, "李四");
  drawText(page, 110, 115, 200, 300, 5, 12, "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转");
  drawText(page, 335, 115, 200, 300, 5, 12, "需要");

}

void CCFlow::createFlow() {

  HPDF_Point pos;
  const char *detail_font_name;

  // Add a new page object.
  HPDF_Page page = addPage("index");

  std::vector<std::string> nodes;
  nodes.push_back("研发部");
  nodes.push_back("研发部");
  nodes.push_back("研发部");
  nodes.push_back("研发部");
  nodes.push_back("研发部");

  float x = HPDF_Page_GetWidth(page)/2;
  float y = 610;
  float radius = 120;

  drawCircle(page, x, y, radius, "客户", nodes);

  addResponsibilities(page);

  // Draw the title
  drawText(page, 250, 800, 500, 300, 5, 12, "这里放标题。");

  // Add the link.
  //addLink(page);
}

void CCFlow::save(const char* fileName) {

  HPDF_SaveToFile(pdf, fileName);

}

