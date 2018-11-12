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
  std::string target;
  int date; // epoch time.
  node_desc_t* nodes;
  int size;

} flow_desc_t;

namespace YAML{
template<>
struct convert<flow_desc_t> {
  static Node encode(const flow_desc_t& flow_desc) {
    Node node;
    node.push_back(flow_desc.owner);
    node.push_back(flow_desc.target);
    node.push_back(flow_desc.date);
    return node;
  }

  static bool decode(const Node& node, flow_desc_t& flow_desc) {
    if(!node.IsSequence() || node.size() != 3) {
      return false;
    }

    flow_desc.owner = node[0].as<double>();
    flow_desc.target = node[1].as<double>();
    flow_desc.date = node[2].as<double>();
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

