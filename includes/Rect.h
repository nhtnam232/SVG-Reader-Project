#ifndef RECT_H
#define RECT_H

#include"myLib.h"
#include"FilledShape.h"
class myRect : public myFilledShape {
	float m_x, m_y, m_width, m_height;
public:
	myRect() : myFilledShape(), m_x(0), m_y(0), m_width(0), m_height(0){}
	void parse(tinyxml2::XMLElement*) override;
	void draw(Gdiplus::Graphics& g) override;
};



#endif 
