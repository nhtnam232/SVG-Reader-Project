#ifndef FILLEDSHAPE_H
#define FILLEDSHAPE_H

#include"myLib.h"
#include"Shape.h"
#include"Color.h"
class myFilledShape : public myShape {
protected:
	Color m_fill;
	float m_fill_opacity;
public:
	myFilledShape() : myShape(), m_fill("none"), m_fill_opacity(1.0f) {}
	virtual void parse(tinyxml2::XMLElement*) override;
};


#endif 
