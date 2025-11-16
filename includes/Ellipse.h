#ifndef ELLIPSE_H
#define ELLIPSE_H	

#include"myLib.h"
#include"FilledShape.h"

class myEllipse : public myFilledShape {
	float m_cx, m_cy, m_rx, m_ry;
public:
	myEllipse() : myFilledShape(), m_cx(0), m_cy(0), m_rx(0), m_ry(0){ }
	void parse(tinyxml2::XMLElement*) override;
};


#endif 
