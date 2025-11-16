#include"Line.h"
void myLine::parse(tinyxml2::XMLElement* node) {
	myShape::parse(node);
	node->QueryFloatAttribute("x1", &m_x1);
	node->QueryFloatAttribute("x2", &m_x2);
	node->QueryFloatAttribute("y1", &m_y1);
	node->QueryFloatAttribute("y2", &m_y2);
}

void myLine::draw(Gdiplus::Graphics& g) {
	Color stroke = m_stroke;
	stroke.setOpacity(m_stroke_opacity);
	Gdiplus::Pen pen(stroke.getColor(), m_stroke_width);
	g.DrawLine(&pen, m_x1, m_y1, m_x2, m_y2);
}