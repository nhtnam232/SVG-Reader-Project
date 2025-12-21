#include"Text.h"
void myText::parse(tinyxml2::XMLElement* node) {
	myFilledShape::parse(node);
	node->QueryFloatAttribute("x", &m_x);
	node->QueryFloatAttribute("y", &m_y);
    node->QueryFloatAttribute("dx", &m_dx);
    node->QueryFloatAttribute("dy", &m_dy);
	node->QueryFloatAttribute("font-size", &m_font_size);
	const char* content = node->GetText();
    const char* textAnchor = node->Attribute("text-anchor");
    if (textAnchor != nullptr) {
        m_textAnchor = textAnchor;
    }
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
wstring UseUtf8ToWstring(const string& str) {
    if (str.empty()) return wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}
void myText::draw(Gdiplus::Graphics& g)
{
    if (m_text.empty())
        return;


    float finalX = m_x + m_dx;
    float finalY = m_y + m_dy;
    // 1. Lưu transform gốc
    Gdiplus::Matrix originalMatrix;
    g.GetTransform(&originalMatrix);

    // 2. Áp transform của text
    Gdiplus::Matrix* transformMatrix = m_transforms.getFinalMatrix();
    if (transformMatrix != nullptr)
        g.MultiplyTransform(transformMatrix);

    //3. Text anchor
    Gdiplus::StringFormat* format = Gdiplus::StringFormat::GenericTypographic()->Clone();

    if (m_textAnchor == "middle") {
        format->SetAlignment(Gdiplus::StringAlignmentCenter);
    }
    else if (m_textAnchor == "end") {
        format->SetAlignment(Gdiplus::StringAlignmentFar);
    }
    else {
        format->SetAlignment(Gdiplus::StringAlignmentNear); 
    }

    // 4. Tính baseline (SVG y là baseline)
    Gdiplus::FontFamily fontFamily(m_font_family.c_str());
    Gdiplus::REAL ascent = fontFamily.GetCellAscent(m_font_style);
    Gdiplus::REAL emHeight = fontFamily.GetEmHeight(m_font_style);
    Gdiplus::REAL baselineOffset = m_font_size * ascent / emHeight;

    Gdiplus::PointF pos(finalX, finalY - baselineOffset);

    // 5. Vẽ text
    wstring wtext = UseUtf8ToWstring(m_text);
    Gdiplus::GraphicsPath path;
    path.AddString(
        wtext.c_str(),
        -1,
        &fontFamily,
        m_font_style,
        m_font_size,
        pos,
        format
    );
    if (m_fill_opacity > 0) {
        Color fill = m_fill;
        fill.setOpacity(m_fill_opacity);
        Gdiplus::SolidBrush brush(fill.getColor());
        g.FillPath(&brush, &path);
    }

    if (m_stroke_width > 0.0f && m_stroke_opacity > 0) {
        Color strokeColor = m_stroke;
        strokeColor.setOpacity(m_stroke_opacity);
        Gdiplus::Pen pen(strokeColor.getColor(), m_stroke_width);
        pen.SetLineJoin(Gdiplus::LineJoinRound); 
        g.DrawPath(&pen, &path);
    }
   

    // 6. Restore transform
    g.SetTransform(&originalMatrix);

    // 7. Giải phóng matrix
    if (transformMatrix != nullptr)
        delete transformMatrix;
    if (format != nullptr) {
        delete format;
    }
}
