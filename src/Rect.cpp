#include"Rect.h"
#include "Parser.h"

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

    Gdiplus::RectF bounds(m_x, m_y, m_width, m_height);

    // 3. Fill
    Gdiplus::Brush* fillBrush = nullptr;

    if (!m_fill_gradient_id.empty()) {
        myGradient* grad = parser.getGradient(m_fill_gradient_id);
        if (grad) fillBrush = grad->createBrush(bounds);
    }
    if (!fillBrush) {
        Color fill = m_fill;
        fill.setOpacity(m_fill_opacity);
        fillBrush = new Gdiplus::SolidBrush(fill.getColor());
    }
    g.FillRectangle(fillBrush, bounds);
    delete fillBrush;

    // 4. Stroke (chỉ vẽ khi hợp lệ)
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
        g.DrawRectangle(&pen, m_x, m_y, m_width, m_height);
        delete strokeBrush;
    }

    // 5. Restore transform
    g.SetTransform(&originalMatrix);

    // 6. Giải phóng matrix
    if (transformMatrix != nullptr)
        delete transformMatrix;
}
