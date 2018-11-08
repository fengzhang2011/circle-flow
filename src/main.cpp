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


#include "ccflow.h"


int main (int argc, char **argv)
{
  CCFlow ccflow;

  ccflow.createFlow();
  ccflow.save("circle-flow.pdf");

  return 0;
}

