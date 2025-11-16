#include"Rect.h"
void myRect::parse(tinyxml2::XMLElement* node) {
	myFilledShape::parse(node);
	node->QueryFloatAttribute("x", &m_x);
	node->QueryFloatAttribute("y", &m_y);
	node->QueryFloatAttribute("width", &m_width);
	node->QueryFloatAttribute("height", &m_height);
}
