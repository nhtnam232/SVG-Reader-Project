// --- Trong Ellipse.cpp ---
#include "Ellipse.h"

void myEllipse::parse(tinyxml2::XMLElement* node) {
	myFilledShape::parse(node);
	node->QueryFloatAttribute("cx", &m_cx);
	node->QueryFloatAttribute("cy", &m_cy);
	node->QueryFloatAttribute("rx", &m_rx);
	node->QueryFloatAttribute("ry", &m_ry);

	// Khởi tạo góc xoay
	m_angle = 0.0f;
	// (Nếu có thuộc tính 'angle' trong XML, bạn có thể QueryFloatAttribute nó ở đây)
}
void myEllipse::draw(Gdiplus::Graphics& g) {
	using namespace Gdiplus; // Sử dụng namespace Gdiplus

	m_fill.setOpacity(m_fill_opacity);
	m_stroke.setOpacity(m_stroke_opacity);
	Gdiplus::SolidBrush brush(m_fill.getColor());

	// Pen cần sử dụng Gdiplus::Color, đảm bảo m_stroke.getColor() trả về Gdiplus::Color
	Gdiplus::Pen stroke_pen(m_stroke.getColor(), m_stroke_width);

	float x = m_cx - m_rx;
	float y = m_cy - m_ry;
	float width = m_rx * 2.0f;
	float height = m_ry * 2.0f;

	// --- Bắt đầu áp dụng Biến đổi (Transform) cho việc xoay ---

	// Lưu trạng thái graphics hiện tại
	GraphicsState state = g.Save();

	// Dịch chuyển gốc tọa độ về tâm của ellipse
	g.TranslateTransform(m_cx, m_cy);

	// Xoay hệ tọa độ theo góc m_angle
	g.RotateTransform(m_angle);

	// Dịch chuyển gốc tọa độ trở lại
	g.TranslateTransform(-m_cx, -m_cy);

	// --- Vẽ hình ellipse (đã được xoay) ---
	g.FillEllipse(&brush, x, y, width, height);
	g.DrawEllipse(&stroke_pen, x, y, width, height);

	// Khôi phục trạng thái Graphics về ban đầu (Loại bỏ phép xoay)
	g.Restore(state);

	// --- Kết thúc áp dụng Biến đổi ---
}
// Scale đồng đều
void myEllipse::transform_scale(float d) {
	m_rx *= d;
	m_ry *= d;
	// Tâm (m_cx, m_cy) không đổi.
}

// Scale không đồng đều
void myEllipse::transform_scale(float sx, float sy) {
	m_rx *= sx;
	m_ry *= sy;
	// Tâm (m_cx, m_cy) không đổi.
}

// Dịch chuyển
void myEllipse::transform_translate(float dx, float dy) {
	m_cx += dx;
	m_cy += dy;
}

// Xoay quanh tâm
void myEllipse::transform_rotate(float angle_degrees) {
	// Cập nhật góc xoay
	m_angle += angle_degrees;

	// (Tùy chọn) Đảm bảo góc luôn nằm trong phạm vi [0, 360)
	// m_angle = fmod(m_angle, 360.0f);
	// if (m_angle < 0) m_angle += 360.0f;
}
