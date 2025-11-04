#ifndef POINTS_H
#define POINTS_H
#include"myLib.h"
class Points {
	// use Vector2f for SFML can render this directly.
	vector<sf::Vector2f> m_points;
public:
	Points() {}
	Points(const char*);
	const vector<sf::Vector2f>& getPoints() const;
};




#endif // !POINTS_H
