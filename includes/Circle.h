#ifndef CIRCLE_H
#define CIRCLE_H

#include"myLib.h"
#include"FilledShape.h"

class Circle : public FilledShape {
	float m_cx, m_cy, m_r;

public:
	Circle() : FilledShape(), m_cx(0), m_cy(0), m_r(0) {};
	void parse(XMLElement*) override;
	void draw(sf::RenderWindow&) override;
};




#endif 
