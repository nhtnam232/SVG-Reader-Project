#include"Rect.h"
void myRect::parse(tinyxml2::XMLElement* node) {
	myFilledShape::parse(node);
	node->QueryFloatAttribute("x", &m_x);
	node->QueryFloatAttribute("y", &m_y);
	node->QueryFloatAttribute("width", &m_width);
	node->QueryFloatAttribute("height", &m_height);
}

void myRect::draw(Gdiplus::Graphics& g) {
    Color fill = m_fill;
    fill.setOpacity(m_fill_opacity);
    Gdiplus::SolidBrush brush(fill.getColor());
    g.FillRectangle(&brush, m_x, m_y, m_width, m_height);

    Color stroke = m_stroke;
    stroke.setOpacity(m_stroke_opacity);
    Gdiplus::Pen pen(stroke.getColor(), m_stroke_width);
    g.DrawRectangle(&pen, m_x, m_y, m_width, m_height);
}
