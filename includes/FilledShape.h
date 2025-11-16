#ifndef FILLEDSHAPE_H
#define FILLEDSHAPE_H

#include"myLib.h"
#include"Shape.h"
#include"Color.h"
class FilledShape : public Shape {
protected:
	Color m_fill;
	float m_fill_opacity;
public:
	FilledShape() : Shape(), m_fill("none"), m_fill_opacity(1.0f) {}
	virtual void parse(tinyxml2::XMLElement*) override;
};


#endif 
