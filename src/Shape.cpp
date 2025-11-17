#include"Shape.h"
void myShape::parse(tinyxml2::XMLElement* node) {
	const char* stroke_value = node->Attribute("stroke");
	if (stroke_value != nullptr) {
		m_stroke = Color(stroke_value);
	}
	node->QueryFloatAttribute("stroke-width", &m_stroke_width);
	node->QueryFloatAttribute("stroke-opacity", &m_stroke_opacity);
}
Color myShape::getStroke() {
	return m_stroke;
}
float myShape::getStrokeWidth() {
	return m_stroke_width;
}
float myShape::getStrokeOpacity() {
	return m_stroke_opacity;
}
void myShape::setStroke(Color other){
	m_stroke = other;
}
void myShape::setStrokeWidth(float other){
	m_stroke_width = other;
}
void myShape::setStrokeOpacity(float other) {
	m_stroke_opacity = other;
}