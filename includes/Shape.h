#ifndef SHAPE_H
#define SHAPE_H
#include"myLib.h"
#include"Color.h"
class myShape
{
protected:
	Color m_stroke;
	float m_stroke_width, m_stroke_opacity;

public:
	myShape() : m_stroke(""), m_stroke_width(1.0f), m_stroke_opacity(1.0f){}
	virtual ~myShape() {};
	virtual void parse(tinyxml2::XMLElement*);
	virtual void draw(Gdiplus::Graphics& g) = 0;
};


#endif 
