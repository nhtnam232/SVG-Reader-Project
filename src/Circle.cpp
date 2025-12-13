#include "Circle.h"
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

    // 4. Fill
    Color fill = m_fill;
    fill.setOpacity(m_fill_opacity);
    Gdiplus::SolidBrush brush(fill.getColor());
    g.FillEllipse(&brush, left, top, width, height);

    // 5. Stroke (chỉ vẽ khi có stroke hợp lệ)
    if (m_stroke.getColor().GetAlpha() != 0 && m_stroke_width != 0)
    {
        Color stroke = m_stroke;
        stroke.setOpacity(m_stroke_opacity);
        Gdiplus::Pen pen(stroke.getColor(), m_stroke_width);
        g.DrawEllipse(&pen, left, top, width, height);
    }

    // 6. Restore transform
    g.SetTransform(&originalMatrix);

    // 7. Giải phóng matrix
    if (transformMatrix != nullptr)
        delete transformMatrix;
}
