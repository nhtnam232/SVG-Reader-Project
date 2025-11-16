#ifndef POLYGON_H
#define POLYGON_H

#include"myLib.h"
#include"FilledShape.h"
#include"Points.h"

class myPolygon : public myFilledShape {
	Points m_points;
public:
	myPolygon() : myFilledShape(){}
	void parse(tinyxml2::XMLElement*) override;
};




#endif 
