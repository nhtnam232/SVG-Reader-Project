#ifndef POLYGON_H
#define POLYGON_H

#include"myLib.h"
#include"FilledShape.h"
#include"Points.h"

class Polygon : public FilledShape {
	Points m_points;
public:
	Polygon() : FilledShape(){}
	void parse(tinyxml2::XMLElement*) override;
};




#endif 
