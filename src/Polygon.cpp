#include "Polygon.h"
#include "Parser.h"

void myPolygon::parse(tinyxml2::XMLElement* node)
{
    if(node == nullptr) {
        std::cerr << "Polygon::parse: NULL node\n";
        return;
    }
    myFilledShape::parse(node);
    m_points = Points(node->Attribute("points"));
}

void myPolygon::draw(Gdiplus::Graphics& g)
{
    Gdiplus::Matrix originalMatrix;
    g.GetTransform(&originalMatrix);

    Gdiplus::Matrix* transformMatrix = m_transforms.getFinalMatrix();
    if (transformMatrix != nullptr)
        g.MultiplyTransform(transformMatrix);

    vector<Gdiplus::PointF> vec = m_points.getPoints();
    Gdiplus::PointF* arrPoints = vec.data();
    int cntPoints = vec.size();

    // bounds
    Gdiplus::GraphicsPath path;
    path.AddPolygon(arrPoints, vec.size());
    Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 0), m_stroke_width);
    Gdiplus::GraphicsPath* p = static_cast<Gdiplus::GraphicsPath*>(path.Clone());
        // NỞ path theo stroke
    p->Widen(&pen);
    Gdiplus::RectF bounds;
    p->GetBounds(&bounds);
    delete p;

    Gdiplus::Brush* fillBrush = nullptr;
    if (!m_fill_gradient_id.empty()) {
        myGradient* grad = parser.getGradient(m_fill_gradient_id);
        if (grad) fillBrush = grad->createBrush(bounds);
    }
    if (!fillBrush) {
        Color fill = m_fill;
        if (fill.getColor().GetAlpha() != 0)
            fill.setOpacity(m_fill_opacity);
        fillBrush = new Gdiplus::SolidBrush(fill.getColor());
    }
    g.FillPolygon(fillBrush, arrPoints, cntPoints);
    delete fillBrush;

    // Transparent thi width, opacity khong con tac dung 
    // stroke_width am van hien thi
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
        g.DrawPolygon(&pen, arrPoints, cntPoints);
        delete strokeBrush;
    }

    g.SetTransform(&originalMatrix);
    if (transformMatrix != nullptr) delete transformMatrix;
}