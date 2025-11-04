#ifndef  COLOR_H
#define COLOR_H
#include"myLib.h"

class Color {
	sf::Color m_color;
public:
	Color() {}
	Color(const string&);
	sf::Color getColor() const;
	void setOpacity(float);
};

#endif // ! COLOR_H
