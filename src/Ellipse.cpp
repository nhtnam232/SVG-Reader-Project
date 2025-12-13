#include"Ellipse.h"
void myEllipse::parse(tinyxml2::XMLElement* node) {
	myFilledShape::parse(node);
	node->QueryFloatAttribute("cx", &m_cx);
	node->QueryFloatAttribute("cy", &m_cy);
	node->QueryFloatAttribute("rx", &m_rx);
	node->QueryFloatAttribute("ry", &m_ry);
}
void myEllipse::draw(Gdiplus::Graphics& g)
{
    // 1. Lưu transform gốc
    Gdiplus::Matrix originalMatrix;
    g.GetTransform(&originalMatrix);

    // 2. Áp transform của ellipse
    Gdiplus::Matrix* transformMatrix = m_transforms.getFinalMatrix();
    if (transformMatrix != nullptr)
        g.MultiplyTransform(transformMatrix);

    // 3. Bounding box ellipse
    float x = m_cx - m_rx;
    float y = m_cy - m_ry;
    float width = 2.0f * m_rx;
    float height = 2.0f * m_ry;

    // 4. Fill
    Color fill = m_fill;
    fill.setOpacity(m_fill_opacity);
    Gdiplus::SolidBrush brush(fill.getColor());
    g.FillEllipse(&brush, x, y, width, height);

    // 5. Stroke (chỉ vẽ khi hợp lệ)
    if (m_stroke.getColor().GetAlpha() != 0 && m_stroke_width != 0)
    {
        Color stroke = m_stroke;
        stroke.setOpacity(m_stroke_opacity);
        Gdiplus::Pen stroke_pen(stroke.getColor(), m_stroke_width);
        g.DrawEllipse(&stroke_pen, x, y, width, height);
    }

    // 6. Restore transform
    g.SetTransform(&originalMatrix);

    // 7. Giải phóng matrix
    if (transformMatrix != nullptr)
        delete transformMatrix;
}
