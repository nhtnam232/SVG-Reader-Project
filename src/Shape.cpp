#include"Shape.h"
void Shape::parse(tinyxml2::XMLElement* node) {
	const char* stroke_value = node->Attribute("stroke");
	if (stroke_value != nullptr) {
		m_stroke = Color(stroke_value);
	}
	node->QueryFloatAttribute("stroke-width", &m_stroke_width);
	node->QueryFloatAttribute("stroke-opacity", &m_stroke_opacity);
}
