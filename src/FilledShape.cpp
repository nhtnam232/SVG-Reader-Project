#include"FilledShape.h"
void myFilledShape::parse(tinyxml2::XMLElement* node) {
	myShape::parse(node);
	const char* fill_value = node->Attribute("fill");
	if (fill_value != nullptr) {
		string fillStr = fill_value;
		if (Color::isUrl(fillStr))
			this->m_fill_gradient_id = Color::extractId(fillStr);
		else {
			this->m_fill = Color(fillStr);
			this->m_fill_gradient_id = "";
		}
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
