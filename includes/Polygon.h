#ifndef POLYGON_H
#define POLYGON_H

#include"myLib.h"
#include"FilledShape.h"


class Polygon : public FilledShape {
	vector<sf::Vector2f> m_points;
public:
	Polygon() : FilledShape(){}
	void parse(XMLElement*) override;
	void draw(sf::RenderWindow&) override;
};




#endif 
