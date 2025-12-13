#ifndef TEXT_H
#define TEXT_H
#include"myLib.h"
#include"FilledShape.h"
class myText : public myFilledShape {
	float m_x, m_y, m_font_size;
	string m_text;
	std::wstring m_font_family;
	Gdiplus::FontStyle m_font_style;
public:
	myText() : myFilledShape(), m_x(0), m_y(0), m_font_size(12), m_text(""), m_font_family(L"Times New Roman"),
		m_font_style(Gdiplus::FontStyleRegular) {}
	void parse(tinyxml2::XMLElement*) override;
	void draw(Gdiplus::Graphics& g) override;
};


#endif 
