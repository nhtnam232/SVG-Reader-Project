#include"Text.h"
void myText::parse(tinyxml2::XMLElement* node) {
	myFilledShape::parse(node);
	node->QueryFloatAttribute("x", &m_x);
	node->QueryFloatAttribute("y", &m_y);
	node->QueryFloatAttribute("font-size", &m_font_size);
	const char* content = node->GetText();
	if (content != nullptr) {
		m_text = content;
	}
}

void myText::draw(Gdiplus::Graphics& g) {
	if (m_text.empty()) {
		return;
	}
	std::wstring fontName = L"Times New Roman";
	Gdiplus::Font font(fontName.c_str(), m_font_size, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

	Color fill = m_fill;
	fill.setOpacity(m_fill_opacity);
	Gdiplus::SolidBrush brush(fill.getColor());

	Gdiplus::FontFamily fontFamily(fontName.c_str());
	Gdiplus::REAL ascent = fontFamily.GetCellAscent(Gdiplus::FontStyleRegular);
	Gdiplus::REAL emHeight = fontFamily.GetEmHeight(Gdiplus::FontStyleRegular);
	Gdiplus::REAL baselineOffset = m_font_size * ascent / emHeight;

	Gdiplus::PointF pos(m_x, m_y - baselineOffset);

	g.DrawString(wstring(m_text.begin(), m_text.end()).c_str(), -1, &font, pos, &brush);
}