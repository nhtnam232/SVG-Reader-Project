#include"Ellipse.h"
void myEllipse::parse(tinyxml2::XMLElement* node) {
	myFilledShape::parse(node);
	node->QueryFloatAttribute("cx", &m_cx);
	node->QueryFloatAttribute("cy", &m_cy);
	node->QueryFloatAttribute("rx", &m_rx);
	node->QueryFloatAttribute("ry", &m_ry);
}
void myEllipse::draw(Gdiplus::Graphics& g) {
	m_fill.setOpacity(m_fill_opacity);
	m_stroke.setOpacity(m_stroke_opacity);
	Gdiplus::SolidBrush brush(m_fill.getColor());
	Gdiplus::Pen stroke_pen(m_stroke.getColor(), m_stroke_width);
	float x = m_cx - m_rx;
	float y = m_cy - m_ry;
	float width = m_rx * 2.0f;
	float height = m_ry * 2.0f;
	g.FillEllipse(&brush, x, y, width, height);
	g.DrawEllipse(&stroke_pen, x, y, width, height);
}
