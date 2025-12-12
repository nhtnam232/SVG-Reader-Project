#include "Polygon.h"

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
    if (transformMatrix != nullptr) g.MultiplyTransform(transformMatrix);

    vector<Gdiplus::PointF> vec = m_points.getPoints();
    Gdiplus::PointF* arrPoints = vec.data();
    int cntPoints = vec.size();

    m_fill.setOpacity(m_fill_opacity);
    Gdiplus::SolidBrush brush(m_fill.getColor());
    g.FillPolygon(&brush, arrPoints, cntPoints);

    // Transparent thi width, opacity khong con tac dung 
    // stroke_width am van hien thi
    if (m_stroke.getColor().GetAlpha() != 0 && m_stroke_width != 0)
    {
        m_stroke.setOpacity(m_stroke_opacity);
        Gdiplus::Pen stroke_pen(m_stroke.getColor(), m_stroke_width);
        g.DrawPolygon(&stroke_pen, arrPoints, cntPoints);
    }  

    g.SetTransform(&originalMatrix);
    if (transformMatrix != nullptr) delete transformMatrix;
}



