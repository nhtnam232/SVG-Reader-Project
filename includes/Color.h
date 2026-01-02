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

	static bool isUrl(const string& str) {
		return (str.find("url(#") == 0);
	}

	static string extractId(const string& str) {
		// Trích xuất "id" từ "url(#id)"
		size_t start = str.find("#") + 1;
		size_t end = str.find(")");
		if (start != string::npos && end != string::npos) {
			return str.substr(start, end - start);
		}
		return "";
	}
};

#endif // ! COLOR_H
