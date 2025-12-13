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

void myText::draw(Gdiplus::Graphics& g)
{
    if (m_text.empty())
        return;

    // 1. Lưu transform gốc
    Gdiplus::Matrix originalMatrix;
    g.GetTransform(&originalMatrix);

    // 2. Áp transform của text
    Gdiplus::Matrix* transformMatrix = m_transforms.getFinalMatrix();
    if (transformMatrix != nullptr)
        g.MultiplyTransform(transformMatrix);

    // 3. Font
    std::wstring fontName = L"Times New Roman";
    Gdiplus::Font font(
        fontName.c_str(),
        m_font_size,
        Gdiplus::FontStyleRegular,
        Gdiplus::UnitPixel
    );

    // 4. Fill
    Color fill = m_fill;
    fill.setOpacity(m_fill_opacity);
    Gdiplus::SolidBrush brush(fill.getColor());

    // 5. Tính baseline (SVG y là baseline)
    Gdiplus::FontFamily fontFamily(fontName.c_str());
    Gdiplus::REAL ascent = fontFamily.GetCellAscent(Gdiplus::FontStyleRegular);
    Gdiplus::REAL emHeight = fontFamily.GetEmHeight(Gdiplus::FontStyleRegular);
    Gdiplus::REAL baselineOffset = m_font_size * ascent / emHeight;

    Gdiplus::PointF pos(m_x, m_y - baselineOffset);

    // 6. Vẽ text
    std::wstring wtext(m_text.begin(), m_text.end());
    g.DrawString(wtext.c_str(), -1, &font, pos, &brush);

    // 7. Restore transform
    g.SetTransform(&originalMatrix);

    // 8. Giải phóng matrix
    if (transformMatrix != nullptr)
        delete transformMatrix;
}
