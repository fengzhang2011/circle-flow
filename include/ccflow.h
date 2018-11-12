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
#include "yaml-cpp/yaml.h"


typedef struct {

  std::string group;
  std::string name;
  std::string input;
  std::string output;

} node_desc_t;

typedef struct {

  std::string owner;
  std::string title;
  std::string target;
  std::string date;
  std::vector<node_desc_t> nodes;

} flow_desc_t;

namespace YAML{

template<>
struct convert<flow_desc_t> {
  static Node encode(const flow_desc_t& flow_desc) {
    Node node;

    node["主题"] = flow_desc.title;
    node["目标"] = flow_desc.target;
    node["总负责人"] = flow_desc.owner;
    node["日期"] = flow_desc.date;
    node["描述"] = flow_desc.nodes;

    return node;
  }

  static bool decode(const Node& node, flow_desc_t& flow_desc) {

    flow_desc.title = node["主题"].as<std::string>();
    flow_desc.target = node["目标"].as<std::string>();
    flow_desc.owner= node["总负责人"].as<std::string>();
    flow_desc.date = node["日期"].as<std::string>();
    flow_desc.nodes = node["描述"].as<std::vector<node_desc_t>>();

    return true;
  }
};

template<>
struct convert<node_desc_t> {
  static Node encode(const node_desc_t& node_desc) {

    Node node;
    node["部门"] = node_desc.group;
    node["负责人"] = node_desc.name;
    node["输入"] = node_desc.input;
    node["输出"] = node_desc.output;

    Node node2;
    node2.push_back(node);
    return node2;

  }

  static bool decode(const Node& node, node_desc_t& node_desc) {

    if(!node.IsSequence() || node.size() != 1) {
      return false;
    }

    node_desc.group = node[0]["部门"].as<std::string>();
    node_desc.name = node[0]["负责人"].as<std::string>();
    node_desc.input = node[0]["输入"].as<std::string>();
    node_desc.output = node[0]["输出"].as<std::string>();

    return true;

  }
};

}

// CCFlow class
class CCFlow {

public:
  CCFlow();
  ~CCFlow();

public:
  HPDF_Page addPage(std::string pageId);
  void createFlow(const flow_desc_t &flow_desc);
  void save(const char* fileName);

private:
  void initialize();
  void finalize();

  // Draw a circle flow.
  void drawCircle(HPDF_Page page, float x, float y, float radius, const flow_desc_t &flow_desc);


private:
  HPDF_Doc pdf;
  HPDF_Font font;

  std::map<std::string, HPDF_Page> pages;

};

#endif // #ifndef _CCFLOW_INCLUDE_H_

