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

#include <stdio.h>
#include <fstream>
#include <string>

int main (int argc, char **argv)
{
  if(argc!=3) {
    printf("Usage:\n  ./circle-flow input.yaml output.pdf\n");
    exit(0);
  }

  const char* inputFileName = argv[1];
  const char* outputFileName = argv[2];

  flow_desc_t flow_desc;
/*
  flow_desc.title= "标题栏";
  flow_desc.target = "客户";
  flow_desc.owner = "负责人";
  flow_desc.date = "2018年11月9日";
  int N = 7;
  for(int i=0; i<N; i++) {
    node_desc_t node;
    node.group = "研发部";
    node.name = "张三";
    node.input = "需要对目标进行拆解，明确每个目标的具体负责人。还需要协调每个组之间的接口，确保整个过程的正常运转";
    node.output = "输出";
    flow_desc.nodes.push_back(node);
  }

  CCFlow ccflow;
  ccflow.createFlow(flow_desc);
  ccflow.save("circle-flow.pdf");

//  YAML::Node node = YAML::Load("start: [1, 3, 0]");
//  Vec3 v = node["start"].as<Vec3>();
//  node["end"] = Vec3(2, -1, 0);
  YAML::Node node;
  node["流程图"] = flow_desc;
  printf("%s\n", Dump(node).c_str());
*/
  std::ifstream ifs(inputFileName);
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );

  YAML::Node node2 = YAML::Load(content.c_str());
  flow_desc_t flow_desc2 = node2["流程图"].as<flow_desc_t>();

  CCFlow ccflow2;
  ccflow2.createFlow(flow_desc2);
  ccflow2.save(outputFileName);

  return 0;
}

