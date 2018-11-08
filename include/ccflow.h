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

#ifndef _CCFLOW_INCLUDE_H_
#define _CCFLOW_INCLUDE_H_

#include <map>
#include <string>
#include <vector>

#include "libharu/hpdf.h"

class CCFlow {

public:
  CCFlow();
  ~CCFlow();

public:
  HPDF_Page addPage(std::string pageId);
  void createFlow();
  void save(const char* fileName);

private:
  void initialize();
  void finalize();

  // Draw a circle flow.
  void drawCircle(HPDF_Page page, float x, float y, float radius, std::string center, std::vector<std::string> nodes);


private:
  HPDF_Doc pdf;
  std::map<std::string, HPDF_Page> pages;

};

#endif // #ifndef _CCFLOW_INCLUDE_H_

