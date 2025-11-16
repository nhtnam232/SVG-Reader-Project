#include"Circle.h"

void Circle::parse(XMLElement* node) {
	FilledShape::parse(node);
	node->QueryFloatAttribute("cx", &m_cx);
	node->QueryFloatAttribute("cy", &m_cy);
	node->QueryFloatAttribute("r", &m_r);
}
void Circle::draw(sf::RenderWindow &window) {
	sf::CircleShape circle(m_r);
	circle.setPosition(m_cx - m_r, m_cy - m_r);
	m_fill.setOpacity(m_fill_opacity);
	circle.setFillColor(m_fill.getColor());
	m_stroke.setOpacity(m_stroke_opacity);
	circle.setOutlineColor(m_stroke.getColor());
	circle.setOutlineThickness(m_stroke_width);
	window.draw(circle);
}