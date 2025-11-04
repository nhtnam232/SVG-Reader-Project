#ifndef POLYLINE_H
#define POLYLINE_H

#include"myLib.h"
#include"FilledShape.h"

class Polyline : public FilledShape {
	vector<sf::Vector2f> m_points;
public:
	Polyline() : FilledShape() {}
	void parse(XMLElement*) override;
	void draw(sf::RenderWindow&) override;
};

#endif 
