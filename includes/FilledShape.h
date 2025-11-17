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
	myFilledShape() : myShape(), m_fill(""), m_fill_opacity(1.0f) {}
	virtual void parse(tinyxml2::XMLElement*) override;
	virtual void draw(Gdiplus::Graphics& g) override = 0;
	Color getFill();
	float getFillOpacity();
	void setFill(Color);
	void setFillOpacity(float);
	bool isFilledShape() override { return true; }
};


#endif 
