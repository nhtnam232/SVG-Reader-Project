#include"Rect.h"

void Rect::parse(XMLElement* node) {
	FilledShape::parse(node);
	node->QueryFloatAttribute("x", &m_x);
	node->QueryFloatAttribute("y", &m_y);
	node->QueryFloatAttribute("width", &m_width);
	node->QueryFloatAttribute("height", &m_height);
}
void Rect::draw(sf::RenderWindow& window) {
	sf::RectangleShape rectangle(sf::Vector2f(m_width, m_height));
	rectangle.setPosition(m_x, m_y);
	m_fill.setOpacity(m_fill_opacity);
	rectangle.setFillColor(m_fill.getColor());
	m_stroke.setOpacity(m_stroke_opacity);
	rectangle.setOutlineColor(m_stroke.getColor());
	rectangle.setOutlineThickness(m_stroke_width);
	window.draw(rectangle);
}