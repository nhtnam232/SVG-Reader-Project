#include"Circle.h"

void myCircle::parse(tinyxml2::XMLElement* node) {
    myFilledShape::parse(node);
    node->QueryFloatAttribute("cx", &m_cx);
    node->QueryFloatAttribute("cy", &m_cy);

    // Đọc bán kính R từ XML. Mặc định Circle/Ellipse có thuộc tính 'r'
    float r = 0;
    node->QueryFloatAttribute("r", &r);

    // Ban đầu, nó là hình tròn, nên Rx = Ry = R
    m_rx = r;
    m_ry = r;

    // Khởi tạo góc xoay ban đầu
    m_angle = 0; // <<< Bổ sung
}

void myCircle::draw(Gdiplus::Graphics& g) {
    // Độ rộng (width) là 2 * Rx, Độ cao (height) là 2 * Ry
    float width = 2 * m_rx;
    float height = 2 * m_ry;

    // Tọa độ góc trên bên trái của hình chữ nhật bao quanh ellipse
    float left = m_cx - m_rx;
    float top = m_cy - m_ry;

    // Lưu trạng thái graphics hiện tại để khôi phục sau
    Gdiplus::GraphicsState state = g.Save();

    // Dịch chuyển gốc tọa độ về tâm của ellipse (m_cx, m_cy)
    g.TranslateTransform(m_cx, m_cy);

    // Xoay hệ tọa độ theo góc m_angle
    g.RotateTransform(m_angle);

    // Dịch chuyển gốc tọa độ trở lại (để vẽ ellipse tại vị trí tương đối)
    g.TranslateTransform(-m_cx, -m_cy);

    // Vẽ Fill
    Color fill = m_fill;
    fill.setOpacity(m_fill_opacity);
    Gdiplus::SolidBrush brush(fill.getColor());
    // Sử dụng FillEllipse
    g.FillEllipse(&brush, left, top, width, height);

    // Vẽ Stroke
    Color stroke = m_stroke;
    stroke.setOpacity(m_stroke_opacity);
    Gdiplus::Pen pen(stroke.getColor(), m_stroke_width);
    // Sử dụng DrawEllipse
    g.DrawEllipse(&pen, left, top, width, height);

    g.Restore(state);
}
//transform_scale không đều -> Tạo hình ellipse

// Hàm scale không đồng đều (Tạo Ellipse)
void myCircle::transform_scale(float sx, float sy) {
    m_rx *= sx;
    m_ry *= sy;
    // Tâm (m_cx, m_cy) không thay đổi vì scale quanh tâm.
}

// Hàm scale đồng đều
void myCircle::transform_scale(float d)
{
    m_rx *= d;
    m_ry *= d;
}

// Hàm dịch chuyển
void myCircle::transform_translate(float dx, float dy) {
    m_cx += dx;
    m_cy += dy;
}

void myCircle::transform_rotate(float angle_degrees) {
    // Xoay ellipse quanh tâm của nó: Chỉ cập nhật góc xoay m_angle.
    m_angle += angle_degrees;

    // (Tùy chọn) Đảm bảo góc luôn nằm trong phạm vi [0, 360)
    // if (m_angle >= 360.0f) {
    //     m_angle = fmod(m_angle, 360.0f);
    // }
    // else if (m_angle < 0.0f) {
    //     m_angle = fmod(m_angle, 360.0f) + 360.0f;
    // }
}