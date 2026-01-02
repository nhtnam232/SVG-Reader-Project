#ifndef _RADIALGRADIENT_H_
#define _RADIALGRADIENT_H_

#include "Gradient.h"
#include "Parser.h"

class myRadialGradient : public myGradient {
    float cx, cy, r, fx, fy;
public:
    myRadialGradient() : cx(0.5f), cy(0.5f), r(0.5f), fx(0.5f), fy(0.5f) {}
    Gdiplus::Brush* createBrush(const Gdiplus::RectF& objectBounds) override;
    void parse(tinyxml2::XMLElement* node) override;
};

#endif // _RADIALGRADIENT_H_
