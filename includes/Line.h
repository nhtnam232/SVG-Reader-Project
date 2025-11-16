#ifndef LINE_H
#define LINE_H

#include"Shape.h"
class Line : public Shape {
	float m_x1, m_x2, m_y1, m_y2;
public:
	Line() : Shape(), m_x1(0), m_x2(0), m_y1(0), m_y2(0) {};
	void parse(tinyxml2::XMLElement*) override;
};


#endif 
