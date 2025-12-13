#include"Rect.h"
void myRect::parse(tinyxml2::XMLElement* node) {
	myFilledShape::parse(node);
	node->QueryFloatAttribute("x", &m_x);
	node->QueryFloatAttribute("y", &m_y);
	node->QueryFloatAttribute("width", &m_width);
	node->QueryFloatAttribute("height", &m_height);
}

void myRect::draw(Gdiplus::Graphics& g)
{
    // 1. Lưu transform gốc
    Gdiplus::Matrix originalMatrix;
    g.GetTransform(&originalMatrix);

    // 2. Áp transform của rect
    Gdiplus::Matrix* transformMatrix = m_transforms.getFinalMatrix();
    if (transformMatrix != nullptr)
        g.MultiplyTransform(transformMatrix);

    // 3. Fill
    Color fill = m_fill;
    fill.setOpacity(m_fill_opacity);
    Gdiplus::SolidBrush brush(fill.getColor());
    g.FillRectangle(&brush, m_x, m_y, m_width, m_height);

    // 4. Stroke (chỉ vẽ khi hợp lệ)
    if (m_stroke.getColor().GetAlpha() != 0 && m_stroke_width != 0)
    {
        Color stroke = m_stroke;
        stroke.setOpacity(m_stroke_opacity);
        Gdiplus::Pen pen(stroke.getColor(), m_stroke_width);
        g.DrawRectangle(&pen, m_x, m_y, m_width, m_height);
    }

    // 5. Restore transform
    g.SetTransform(&originalMatrix);

    // 6. Giải phóng matrix
    if (transformMatrix != nullptr)
        delete transformMatrix;
}
