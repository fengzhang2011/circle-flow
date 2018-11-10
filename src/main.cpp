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
  flow_desc_t flow_desc;
  flow_desc.owner = "负责人";
  flow_desc.target = "客户";
  flow_desc.date = 1541773776; // epoch: 2018年 11月 09日 星期五 22:29
  int N = 7;
  node_desc_t* nodes = new node_desc_t[N];
  flow_desc.nodes = nodes;
  flow_desc.size = N;
  for(int i=0; i<N; i++) {
    nodes[i].group = "研发部";
    nodes[i].name = "张三";
    nodes[i].input = "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转";
    nodes[i].output = "输出";
  }

  CCFlow ccflow;
  ccflow.createFlow(flow_desc);
  ccflow.save("circle-flow.pdf");

  delete []nodes;
  flow_desc.nodes = NULL;
  flow_desc.size = 0;

  return 0;
}

