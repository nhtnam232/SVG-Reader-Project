#include "Circle.h"
#include "Parser.h"

void myCircle::parse(tinyxml2::XMLElement* node) {
    myFilledShape::parse(node);
    node->QueryFloatAttribute("cx", &m_cx);
    node->QueryFloatAttribute("cy", &m_cy);
    node->QueryFloatAttribute("r", &m_r);
}
void myCircle::draw(Gdiplus::Graphics& g)
{
    // 1. Lưu lại transform gốc
    Gdiplus::Matrix originalMatrix;
    g.GetTransform(&originalMatrix);

    // 2. Áp transform của circle (nếu có)
    Gdiplus::Matrix* transformMatrix = m_transforms.getFinalMatrix();
    if (transformMatrix != nullptr)
        g.MultiplyTransform(transformMatrix);

    // 3. Tính bounding box của circle
    float left = m_cx - m_r;
    float top = m_cy - m_r;
    float width = 2 * m_r;
    float height = 2 * m_r;
    Gdiplus::RectF bounds(left, top, width, height);
    
    // 4. Fill
    Gdiplus::Brush* fillBrush = nullptr;
    // Kiểm tra xem có dùng Gradient không (m_fill_gradient_id là string mới thêm vào class)
    if (!m_fill_gradient_id.empty()) {
        myGradient* grad = parser.getGradient(m_fill_gradient_id);
        if (grad) fillBrush = grad->createBrush(bounds);
    }
    // Nếu không có gradient hoặc tìm không thấy, dùng Solid Color
    if (!fillBrush) {
        Color fill = m_fill;
        fill.setOpacity(m_fill_opacity);
        fillBrush = new Gdiplus::SolidBrush(fill.getColor());
    }
    g.FillEllipse(fillBrush, left, top, width, height);
    delete fillBrush;

    // 5. Stroke (chỉ vẽ khi có stroke hợp lệ)
    if (m_stroke_width != 0 && (!m_stroke_gradient_id.empty() || m_stroke.getColor().GetAlpha() != 0))
    {
        Gdiplus::Brush* strokeBrush = nullptr;
        if (!m_stroke_gradient_id.empty()) {
            myGradient* grad = parser.getGradient(m_stroke_gradient_id);
            if (grad) strokeBrush = grad->createBrush(bounds);
        }

        if (!strokeBrush) {
            Color stroke = m_stroke;
            stroke.setOpacity(m_stroke_opacity);
            strokeBrush = new Gdiplus::SolidBrush(stroke.getColor());
        }

        Gdiplus::Pen pen(strokeBrush, m_stroke_width);
        g.DrawEllipse(&pen, left, top, width, height);
        delete strokeBrush;
    }

    // 6. Restore transform
    g.SetTransform(&originalMatrix);

    // 7. Giải phóng matrix
    if (transformMatrix != nullptr)
        delete transformMatrix;
}
