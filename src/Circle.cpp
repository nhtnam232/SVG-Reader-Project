#include"Circle.h"
void Circle::parse(tinyxml2::XMLElement* node) {
	FilledShape::parse(node);
	node->QueryFloatAttribute("cx", &m_cx);
	node->QueryFloatAttribute("cy", &m_cy);
	node->QueryFloatAttribute("r", &m_r);
}
