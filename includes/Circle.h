#ifndef CIRCLE_H
#define CIRCLE_H

#include"myLib.h"
#include"FilledShape.h"

class myCircle : public myFilledShape {
	float m_cx, m_cy, m_r;

public:
	myCircle() : myFilledShape(), m_cx(0), m_cy(0), m_r(0) {};
	void parse(tinyxml2::XMLElement*) override;
};




#endif 
