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


#include "libharu/hpdf.h"

class CCFlow {

public:
  CCFlow();
  ~CCFlow();

public:
  void addPage();
  void save(const char* fileName);

private:
  void initialize();
  void finalize();

private:
  HPDF_Doc pdf;

};

#endif // #ifndef _CCFLOW_INCLUDE_H_

