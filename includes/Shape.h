#ifndef SHAPE_H
#define SHAPE_H
#include"myLib.h"


class Shape
{
protected:
	string m_stroke;
	float m_stroke_width, m_stroke_opacity;

public:
	Shape() : m_stroke("none"), m_stroke_width(1.0f), m_stroke_opacity(1.0f){}
	virtual ~Shape() {};
	virtual void parse(XMLElement*);
	virtual void draw(sf::RenderWindow&) = 0;
};


#endif 
