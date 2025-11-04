#ifndef TEXT_H
#define TEXT_H
#include"myLib.h"
#include"FilledShape.h"
class Text : public FilledShape {
	float m_x, m_y, m_font_size;
	string m_text;
public:
	Text() : FilledShape(), m_x(0), m_y(0), m_font_size(12), m_text("") {}
	void parse(XMLElement*) override;
	void draw(sf::RenderWindow&) override;
};


#endif 
