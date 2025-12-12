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

	// Khởi tạo các thuộc tính biến đổi
	m_angle = 0.0f;
	m_scale_x = 1.0f;
	m_scale_y = 1.0f;
}

void myText::draw(Gdiplus::Graphics& g) {
    using namespace Gdiplus;

    if (m_text.empty()) {
        return;
    }

    // 1. Chuẩn bị Text và Font
    std::wstring fontName = L"Times New Roman";
    // Lưu ý: Không scale font_size ở đây, mà để GDI+ Matrix xử lý scale
    Font font(fontName.c_str(), m_font_size, FontStyleRegular, UnitPixel);

    ::Color fill = m_fill;
    fill.setOpacity(m_fill_opacity);
    SolidBrush brush(fill.getColor());

    FontFamily fontFamily(fontName.c_str());
    REAL ascent = fontFamily.GetCellAscent(FontStyleRegular);
    REAL emHeight = fontFamily.GetEmHeight(FontStyleRegular);
    REAL baselineOffset = m_font_size * ascent / emHeight;

    // Vị trí đặt văn bản (PointF)
    PointF pos(m_x, m_y - baselineOffset);

    // --- Bắt đầu áp dụng Biến đổi (Transform) ---

    GraphicsState state = g.Save();

    // Điểm neo cho xoay và scale là (m_x, m_y - baselineOffset)
    float anchor_x = m_x;
    float anchor_y = m_y - baselineOffset;

    // 1. Dịch chuyển gốc tọa độ về điểm neo (tâm xoay/scale)
    g.TranslateTransform(anchor_x, anchor_y);

    // 2. Áp dụng Scale (cả theo X và Y)
    // Hệ số scale đã được tích lũy trong m_scale_x, m_scale_y
    g.ScaleTransform(m_scale_x, m_scale_y);

    // 3. Xoay hệ tọa độ theo góc m_angle
    g.RotateTransform(m_angle);

    // 4. Dịch chuyển gốc tọa độ trở lại
    g.TranslateTransform(-anchor_x, -anchor_y);

    // --- Vẽ văn bản ---
    // Vẽ văn bản tại vị trí gốc (x, y) trên hệ tọa độ đã bị biến đổi.
    std::wstring wtext(m_text.begin(), m_text.end());
    g.DrawString(wtext.c_str(), -1, &font, pos, &brush);

    // Khôi phục trạng thái Graphics về ban đầu
    g.Restore(state);

    // --- Kết thúc áp dụng Biến đổi ---
}

// Dịch chuyển
void myText::transform_translate(float dx, float dy) {
    m_x += dx;
    m_y += dy;
}

// Xoay quanh điểm gốc (m_x, m_y)
void myText::transform_rotate(float angle_degrees) {
    m_angle += angle_degrees;
    // (Có thể thêm logic chuẩn hóa góc nếu cần)
}

// Scale không đồng đều (Tích lũy hệ số scale)
void myText::transform_scale(float sx, float sy) {
    // Tích lũy hệ số scale (nhân dồn)
    m_scale_x *= sx;
    m_scale_y *= sy;

    // Lưu ý: Font size (m_font_size) giữ nguyên vì scale được áp dụng qua GDI+ Matrix
}

// Scale đồng đều
void myText::transform_scale(float d) {
    transform_scale(d, d);
}