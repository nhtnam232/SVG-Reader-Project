#include"Circle.h"
void myCircle::parse(tinyxml2::XMLElement* node) {
	myFilledShape::parse(node);
	node->QueryFloatAttribute("cx", &m_cx);
	node->QueryFloatAttribute("cy", &m_cy);
	node->QueryFloatAttribute("r", &m_r);
}

void myCircle::draw(Gdiplus::Graphics& g) {
    float left = m_cx - m_r;
    float top = m_cy - m_r;
    float width = 2 * m_r;
    float height = 2 * m_r;

    Color fill = m_fill;
    fill.setOpacity(m_fill_opacity);
    Gdiplus::SolidBrush brush(fill.getColor());
    g.FillEllipse(&brush, left, top, width, height);
    
    Color stroke = m_stroke;
    stroke.setOpacity(m_stroke_opacity);
    Gdiplus::Pen pen(stroke.getColor(), m_stroke_width);
    g.DrawEllipse(&pen, left, top, width, height);
}
