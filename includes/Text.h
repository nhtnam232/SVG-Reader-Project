#ifndef TEXT_H
#define TEXT_H
#include"myLib.h"
#include"FilledShape.h"
class myText : public myFilledShape {
	float m_x, m_y, m_dx, m_dy, m_font_size;
	string m_text;
	string m_textAnchor;
	std::wstring m_font_family;
	Gdiplus::FontStyle m_font_style;
public:
	myText() : myFilledShape(), m_x(0), m_y(0), m_dx(0), m_dy(0), m_font_size(12), m_text(""),m_textAnchor("start"), m_font_family(L"Times New Roman"),
		m_font_style(Gdiplus::FontStyleRegular) {}
	void parse(tinyxml2::XMLElement*) override;
	void draw(Gdiplus::Graphics& g) override;
	void setFontFamily(const char*);
	void setFontSize(float);
	void setFontStyle(const char*, const char*);
};


#endif 
