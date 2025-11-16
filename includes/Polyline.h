#ifndef POLYLINE_H
#define POLYLINE_H

#include"myLib.h"
#include"FilledShape.h"
#include"Points.h"

class myPolyline : public myFilledShape {
	Points m_points;
public:
	myPolyline() : myFilledShape() {}
	void parse(tinyxml2::XMLElement*) override;
};

#endif 
