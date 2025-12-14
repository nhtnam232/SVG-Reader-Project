#ifndef SHAPE_H
#define SHAPE_H
#include"myLib.h"
#include"Color.h"
#include "Transform.h"
class myShape
{
protected:
	Color m_stroke;
	float m_stroke_width, m_stroke_opacity;
	myListTransform m_transforms;

public:
	myShape() : m_stroke("none"), m_stroke_width(0.0f), m_stroke_opacity(1.0f){}
	virtual ~myShape() {};
	virtual void parse(tinyxml2::XMLElement*);
	virtual void draw(Gdiplus::Graphics& g) = 0;
	Color getStroke();
	float getStrokeWidth();
	float getStrokeOpacity();
	void setStroke(Color);
	void setStrokeWidth(float);
	void setStrokeOpacity(float);
	virtual bool isFilledShape() { return false; }
};


#endif 
