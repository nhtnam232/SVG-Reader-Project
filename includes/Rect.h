#ifndef RECT_H
#define RECT_H

#include"myLib.h"
#include"FilledShape.h"
class Rect : public FilledShape {
	float m_x, m_y, m_width, m_height;
public:
	Rect() : FilledShape(), m_x(0), m_y(0), m_width(0), m_height(0){}
	void parse(tinyxml2::XMLElement*) override;
};



#endif 
