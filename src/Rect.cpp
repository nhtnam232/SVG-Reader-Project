#include"Rect.h"
#include "Rect.h"

void myRect::parse(tinyxml2::XMLElement* node) {
    myFilledShape::parse(node);
    node->QueryFloatAttribute("x", &m_x);
    node->QueryFloatAttribute("y", &m_y);
    node->QueryFloatAttribute("width", &m_width);
    node->QueryFloatAttribute("height", &m_height);

    // Tính toán tâm hình chữ nhật
    m_cx = m_x + m_width / 2.0f;
    m_cy = m_y + m_height / 2.0f;

    // Khởi tạo góc xoay
    m_angle = 0.0f;
}
//co the su dung using MyColor = ::Color;
void myRect::draw(Gdiplus::Graphics& g) {

    Color fill = m_fill;
    fill.setOpacity(m_fill_opacity);
    Gdiplus::SolidBrush brush(fill.getColor());

    Color stroke = m_stroke;
    stroke.setOpacity(m_stroke_opacity);
    Gdiplus::Pen pen(stroke.getColor(), m_stroke_width);

    // --- Bắt đầu áp dụng Biến đổi (Transform) cho việc xoay ---

    Gdiplus::GraphicsState state = g.Save();

    // Dịch chuyển gốc tọa độ về tâm của hình chữ nhật
    g.TranslateTransform(m_cx, m_cy);

    // Xoay hệ tọa độ theo góc m_angle
    g.RotateTransform(m_angle);

    // Dịch chuyển gốc tọa độ trở lại
    g.TranslateTransform(-m_cx, -m_cy);

    // --- Vẽ hình chữ nhật (đã được xoay) ---
    // Sử dụng tọa độ gốc (x, y) để vẽ trên hệ tọa độ đã xoay
    g.FillRectangle(&brush, m_x, m_y, m_width, m_height);
    g.DrawRectangle(&pen, m_x, m_y, m_width, m_height);

    // Khôi phục trạng thái Graphics về ban đầu
    g.Restore(state);

    // --- Kết thúc áp dụng Biến đổi ---
}

void myRect::transform_translate(float dx, float dy) {
    // Cập nhật vị trí góc trên bên trái
    m_x += dx;
    m_y += dy;

    // Cập nhật vị trí tâm
    m_cx += dx;
    m_cy += dy;
}

// Scale không đồng đều (sx, sy)
void myRect::transform_scale(float sx, float sy) {
    // 1. Cập nhật kích thước
    m_width *= sx;
    m_height *= sy;

    // 2. Tính toán lại vị trí góc trên bên trái (m_x, m_y)
    // Tâm (m_cx, m_cy) giữ nguyên, kích thước mới.
    m_x = m_cx - m_width / 2.0f;
    m_y = m_cy - m_height / 2.0f;
}

// Scale đồng đều (d)
void myRect::transform_scale(float d) {
    transform_scale(d, d); // Gọi hàm scale không đồng đều với sx = sy = d
}

void myRect::transform_rotate(float angle_degrees) {
    // Chỉ cập nhật góc xoay
    m_angle += angle_degrees;

    // (Tùy chọn) Chuẩn hóa góc:
    // m_angle = fmod(m_angle, 360.0f);
    // if (m_angle < 0) m_angle += 360.0f;
}
