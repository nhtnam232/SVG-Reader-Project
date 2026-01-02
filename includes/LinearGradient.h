#ifndef LINEAR_GRADIENT_H
#define LINEAR_GRADIENT_H

#include "Gradient.h"
#include "Parser.h"

class myLinearGradient : public myGradient {
	float m_x1, m_y1, m_x2, m_y2;
public:
	myLinearGradient() : m_x1(0.0f), m_y1(0.0f), m_x2(1.0f), m_y2(0.0f) {}
	Gdiplus::Brush* createBrush(const Gdiplus::RectF& objectBounds) override;
	void parse(tinyxml2::XMLElement*);
};

#endif