#ifndef  COLOR_H
#define COLOR_H
#include"myLib.h"

class Color {
	Gdiplus::Color m_color;
	static map<string, Gdiplus::Color> color_map;
	static void loadColor(const string& filename);
public:
	Color() {}
	Color(const string&);
	Gdiplus::Color getColor() const;
	void setOpacity(float);
};

#endif // ! COLOR_H
