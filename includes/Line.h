#ifndef LINE_H
#define LINE_H

#include"Shape.h"
class myLine : public myShape {
	float m_x1, m_x2, m_y1, m_y2;
public:
	myLine() : myShape(), m_x1(0), m_x2(0), m_y1(0), m_y2(0) {};
	void parse(tinyxml2::XMLElement*) override;
	void draw(Gdiplus::Graphics& g) override;
};


#endif 
