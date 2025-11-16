#ifndef  COLOR_H
#define COLOR_H
#include"myLib.h"

class Color {
	Gdiplus::Color m_color;
public:
	Color() {}
	Color(const string&);
	Gdiplus::Color getColor() const;
	void setOpacity(float);
};

#endif // ! COLOR_H
