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

HPDF_Page CCFlow::addPage(std::string pageId) {

  HPDF_Page page = HPDF_AddPage(pdf);

  // Change the DPI. (The following statement changes 72dpi to 300 dpi.)
  // HPDF_Page_Concat (page, 72.0f / 300.0f, 0, 0, 72.0f / 300.0f, 0, 0);

  // Set the page size: A4 by default.
  HPDF_Page_SetHeight (page, 72*11.69);
  HPDF_Page_SetWidth (page, 72*8.27);

  // Draw grid to the page.
  hpdfShowGrid(pdf, page);

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

void drawSupervisor(HPDF_Page page) {

  HPDF_Page_SetLineWidth (page, 1);
  HPDF_Rect rect;
  rect.left = 50;
  rect.top = 440;
  rect.right = 550;
  rect.bottom = 410;

  HPDF_Page_Rectangle (page, rect.left, rect.bottom, rect.right - rect.left,
                rect.top - rect.bottom);

  HPDF_Page_Stroke (page);

  drawText(page, 50, 440, 200, 50, 5, 12, "总负责人： XXX");
  drawText(page, 370, 440, 200, 50, 5, 12, "日期： 2018年11月09日");

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

  drawTable(page);

  // header
  drawText(page, 50, 410, 70, 300, 5, 12, "责任人");
  drawText(page, 120, 410, 200, 300, 5, 12, "输入");
  drawText(page, 330, 410, 200, 300, 5, 12, "输出");

  // first role
  drawText(page, 50, 380, 70, 300, 5, 12, "研发部");
  drawText(page, 50, 360, 70, 300, 5, 12, "张三");
  drawText(page, 120, 380, 200, 300, 5, 12, "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转");
  drawText(page, 330, 380, 200, 300, 5, 12, "需要");

  // second role
  drawText(page, 50, 310, 70, 300, 5, 12, "研发部");
  drawText(page, 50, 290, 70, 300, 5, 12, "李四");
  drawText(page, 120, 310, 200, 300, 5, 12, "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转");
  drawText(page, 330, 310, 200, 300, 5, 12, "需要");

  // third role
  drawText(page, 50, 240, 70, 300, 5, 12, "研发部");
  drawText(page, 50, 220, 70, 300, 5, 12, "李四");
  drawText(page, 120, 240, 200, 300, 5, 12, "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转");
  drawText(page, 330, 240, 200, 300, 5, 12, "需要");

  // forth role
  drawText(page, 50, 170, 70, 300, 5, 12, "研发部");
  drawText(page, 50, 150, 70, 300, 5, 12, "李四");
  drawText(page, 120, 170, 200, 300, 5, 12, "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转");
  drawText(page, 330, 170, 200, 300, 5, 12, "需要");

  // fifth role
  drawText(page, 50, 100, 70, 300, 5, 12, "研发部");
  drawText(page, 50, 80, 70, 300, 5, 12, "李四");
  drawText(page, 120, 100, 200, 300, 5, 12, "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转");
  drawText(page, 330, 100, 200, 300, 5, 12, "需要");

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

  // Add the supervisor.
  drawSupervisor(page);
  
  // Add the link.
  //addLink(page);
}

void CCFlow::save(const char* fileName) {

  HPDF_SaveToFile(pdf, fileName);

}

