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
    const char* fontFamilyAttr = node->Attribute("font-family");
    setFontFamily(fontFamilyAttr);
    const char* fontStyleAttr = node->Attribute("font-style");
    const char* fontWeightAttr = node->Attribute("font-weight");
    setFontStyle(fontStyleAttr, fontWeightAttr);
}
void myText::setFontSize(float fontSize) {
    m_font_size = fontSize;
}
void myText::setFontFamily(const char* fontFamilyAttr) {
    string fontStr = "";
    if (fontFamilyAttr != nullptr) {
        fontStr = fontFamilyAttr;
    }
    stringstream ss(fontStr);
    vector<wstring> fontFamily;
    string token = "";
    while (getline(ss, token, ',')) {
        if (!token.empty() && token[0] == ' ') {
            token.erase(token.begin());
        }
        if ((token.front() == '\'' && token.back() == '\'') ||
            (token.front() == '"' && token.back() == '"')) {
            token = token.substr(1, token.size() - 2);
        }
        std::wstring wtoken(token.begin(), token.end());
        fontFamily.push_back(wtoken);
    }
    for (auto& wtoken : fontFamily) {
        if (wtoken == L"sans-serif") {
            wtoken = L"Arial";
        }
        else if (wtoken == L"monospace") {
            wtoken = L"Consolas";
        }
        else if (wtoken == L"cursive") {
            wtoken = L"Comic Sans MS";
        }
        else if (wtoken == L"fantasy") {
            wtoken = L"Impact";
        }
        Gdiplus::FontFamily family(wtoken.c_str());
        if (family.IsAvailable()) {
            m_font_family = wtoken;
            break;
        }
    }
}
void myText::setFontStyle(const char* rawStyle, const char* rawWeight) {
    bool finalItalic = false;

    if (rawStyle != nullptr) {
        string fs = rawStyle;
        if (fs == "italic" || fs == "oblique") {
            finalItalic = true;
        }
    }
    else {
        if (m_font_style == Gdiplus::FontStyleItalic || m_font_style == Gdiplus::FontStyleBoldItalic) {
            finalItalic = true;
        }
    }

    bool finalBold = false;

    if (rawWeight != nullptr) {
        string fw = rawWeight;
        if (isdigit(fw[0])) {
            if (stoi(fw) >= 600) finalBold = true;
        }
        else if (fw == "bold" || fw == "bolder") {
            finalBold = true;
        }
    }
    else {
        if (m_font_style == Gdiplus::FontStyleBold || m_font_style == Gdiplus::FontStyleBoldItalic) {
            finalBold = true;
        }
    }

    if (finalBold && finalItalic) {
        m_font_style = Gdiplus::FontStyleBoldItalic;
    }
    else if (finalBold) {
        m_font_style = Gdiplus::FontStyleBold;
    }
    else if (finalItalic) {
        m_font_style = Gdiplus::FontStyleItalic;
    }
    else {
        m_font_style = Gdiplus::FontStyleRegular;
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
    Gdiplus::Font font(
        m_font_family.c_str(),
        m_font_size,
        m_font_style,
        Gdiplus::UnitPixel
    );

    // 4. Fill
    Color fill = m_fill;
    fill.setOpacity(m_fill_opacity);
    Gdiplus::SolidBrush brush(fill.getColor());

    // 5. Tính baseline (SVG y là baseline)
    Gdiplus::FontFamily fontFamily(m_font_family.c_str());
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
