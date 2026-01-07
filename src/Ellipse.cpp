#include"Ellipse.h"
#include "Parser.h"

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
    Gdiplus::RectF ellipseRect(x, y, width, height);
    Gdiplus::GraphicsPath path;
    path.AddEllipse(ellipseRect);
    Gdiplus::Pen pen(m_stroke.getColor(), m_stroke_width);
    Gdiplus::GraphicsPath* p = static_cast<Gdiplus::GraphicsPath*>(path.Clone());
    p->Widen(&pen);
    Gdiplus::RectF bounds;
    p->GetBounds(&bounds);
    delete p;

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
        if (fill.getColor().GetAlpha() != 0)
            fill.setOpacity(m_fill_opacity);
        fillBrush = new Gdiplus::SolidBrush(fill.getColor());
    }
    g.FillEllipse(fillBrush, x, y, width, height);
    delete fillBrush;

    // 5. Stroke (chỉ vẽ khi hợp lệ)
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
        g.DrawEllipse(&pen, x, y, width, height);
        delete strokeBrush;
    }

    // 6. Restore transform
    g.SetTransform(&originalMatrix);

    // 7. Giải phóng matrix
    if (transformMatrix != nullptr)
        delete transformMatrix;
}
