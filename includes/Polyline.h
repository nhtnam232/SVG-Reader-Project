#ifndef POLYLINE_H
#define POLYLINE_H

#include"myLib.h"
#include"FilledShape.h"
#include"Points.h"

class Polyline : public FilledShape {
	Points m_points;
public:
	Polyline() : FilledShape() {}
	void parse(XMLElement*) override;
	void draw(sf::RenderWindow&) override;
};

#endif 
