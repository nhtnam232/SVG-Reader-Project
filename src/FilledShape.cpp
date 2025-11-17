#include"FilledShape.h"
void myFilledShape::parse(tinyxml2::XMLElement* node) {
	myShape::parse(node);
	const char* fill_value = node->Attribute("fill");
	if (fill_value != nullptr) {
		m_fill = Color(fill_value);
	}
	node->QueryFloatAttribute("fill-opacity", &m_fill_opacity);
}
Color myFilledShape::getFill() {
	return m_fill;
}
float myFilledShape::getFillOpacity() {
	return m_fill_opacity;
}
void myFilledShape::setFill(Color other) {
	m_fill = other;
}
void myFilledShape::setFillOpacity(float other) {
	m_fill_opacity = other;
}
