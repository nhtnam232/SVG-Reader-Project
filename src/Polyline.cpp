#include"Polyline.h"

void myPolyline::parse(tinyxml2::XMLElement* node)
{
    myFilledShape::parse(node);
    m_points = Points(node->Attribute("points"));
}

void myPolyline::draw(Gdiplus::Graphics& g)
{
    vector<Gdiplus::PointF> vec = m_points.getPoints();
    Gdiplus::PointF* arrPoints = vec.data();
    int cntPoints = vec.size();

    m_fill.setOpacity(m_fill_opacity);
    Gdiplus::SolidBrush brush(m_fill.getColor());
    g.FillPolygon(&brush, arrPoints, cntPoints);

    if (m_stroke.getColor().GetAlpha() != 0) // Transparent
    {
        m_stroke.setOpacity(m_stroke_opacity);
        Gdiplus::Pen stroke_pen(m_stroke.getColor(), m_stroke_width);
        g.DrawLines(&stroke_pen, arrPoints, cntPoints);
    }
}


