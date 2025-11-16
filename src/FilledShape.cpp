#include"FilledShape.h"
void FilledShape::parse(tinyxml2::XMLElement* node) {
	Shape::parse(node);
	const char* fill_value = node->Attribute("fill");
	if (fill_value != nullptr) {
		m_fill = Color(fill_value);
	}
	node->QueryFloatAttribute("fill-opacity", &m_fill_opacity);
}
