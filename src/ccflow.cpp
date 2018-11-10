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

void CCFlow::drawCircle(HPDF_Page page, float x, float y, float radius, const flow_desc_t &flow_desc) {

  int N = flow_desc.size;

  color_t colors[] = { color_cfn_1, color_cfn_2, color_cfn_3, color_cfn_4, color_cfn_5, color_cfn_6, color_cfn_7, color_cfn_8 };

  for(int i=0; i<N; i++)
  {
    std::string text = flow_desc.nodes[i].group;
    if( text.empty() ) {
      text = flow_desc.nodes[i].name;
    }

    float angle = (90 - i*360/N)*M_PI/180;
    // printf("angle=%f\n", angle*180/M_PI);
    float nx = radius * cos(angle) + x;
    float ny = radius * sin(angle) + y;
    drawNode(pdf, page, nx, ny, colors[i].r, colors[i].g, colors[i].b, text.c_str());

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

    drawNodeArc(page, x, y, radius, 90-1.0*arcStart*180/M_PI, 90-1.0*arcEnd*180/M_PI);
    // drawNodeArc(page, x, y, 1.1*radius, 90-1.0*arcStart*180/M_PI, 90-1.0*arcEnd*180/M_PI);
    // drawNodeArc(page, x, y, 0.9*radius, 90-1.0*arcEnd*180/M_PI, 90-1.0*arcStart*180/M_PI, false);
  }

  drawNode(pdf, page, x, y, color_cfn_center.r, color_cfn_center.g, color_cfn_center.b, flow_desc.target.c_str());

}

// Draw table.
void drawTable(HPDF_Page page, float left, float top, float bottom, float right, int rows)
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

  // draw rows
  if(rows>1) {
    float v_space = 1.0 * (top-50-bottom) / rows;
    for(int i=0; i<rows; i++) {
      HPDF_Page_MoveTo(page, left, bottom+v_space*(i+1));
      HPDF_Page_LineTo(page, right, bottom+v_space*(i+1));
    }
  }

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

void addResponsibilities(HPDF_Page page, float left, float top, float bottom, float right, float margin, float fontSize, const flow_desc_t &flow_desc) {

  int rows = flow_desc.size;

  drawTable(page, left, top, bottom, right, rows);

  // Add the supervisor.
  std::string supervisor = "总负责人： "+flow_desc.owner;
  std::string date = "日期： 2018年11月09日";
  drawText(page, left, top, 200, 50, margin, fontSize, supervisor.c_str());
  drawText(page, right-135, top, 200, 50, margin, fontSize, date.c_str());

  // header
  drawText(page, left, top-25, 70, 300, margin, fontSize, "责任人");
  drawText(page, left+60, top-25, 200, 300, margin, fontSize, "输入");
  drawText(page, left+285, top-25, 200, 300, margin, fontSize, "输出");

  // draw rows
  if(rows>1) {
    float v_space = 1.0 * (top-50-bottom) / rows;
    for(int i=rows-1; i>=0; i--) {
      float row_top = bottom+v_space*(i+1);
      node_desc_t node = flow_desc.nodes[rows-1-i];
      if(node.group.empty()) {
        drawText(page, left, row_top, 70, v_space, margin, fontSize, node.name.c_str());
      } else {
        drawText(page, left, row_top, 70, v_space, margin, fontSize, node.group.c_str());
        drawText(page, left, row_top-20, 70, v_space, margin, fontSize, node.name.c_str());
      }
      drawText(page, left+60, row_top, 200, v_space, margin, fontSize, node.input.c_str());
      drawText(page, left+285, row_top, 200, v_space, margin, fontSize, node.output.c_str());
    }
  }

}

void CCFlow::createFlow(const flow_desc_t &flow_desc) {

  HPDF_Point pos;
  const char *detail_font_name;

  // Add a new page object.
  HPDF_Page page = addPage("index");

  float x = HPDF_Page_GetWidth(page)/2;
  float y = 610;
  float radius = 120;

  drawCircle(page, x, y, radius, flow_desc);

  float left = 50;
  float top = 440;
  float bottom = 40;
  float right = 550;

  float margin = 5;
  float fontSize = 12;

  addResponsibilities(page, left, top, bottom, right, margin, fontSize, flow_desc);

  // Draw the title
  HPDF_Page_SetFontAndSize(page, font, 16);
  drawText(page, 250, 810, 500, 300, 5, 16, "这里放标题。");
  HPDF_Page_SetFontAndSize(page, font, 12);

  // Add the link.
  //addLink(page);
}

void CCFlow::save(const char* fileName) {

  HPDF_SaveToFile(pdf, fileName);

}

