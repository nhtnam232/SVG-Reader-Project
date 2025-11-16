#include"Polyline.h"

void myPolyline::parse(tinyxml2::XMLElement* node)
{
    if(node == nullptr) {
        std::cerr << "Polyline::parse: NULL node\n";
        return;
    }
    myFilledShape::parse(node);
    m_points = Points(node->Attribute("points"));
}
void myPolyline::draw(Gdiplus::Graphics& g) {
    m_fill.setOpacity(m_fill_opacity);
    m_stroke.setOpacity(m_stroke_opacity);

    Gdiplus::SolidBrush brush(m_fill.getColor());
    Gdiplus::Pen stroke_pen(m_stroke.getColor(), m_stroke_width);
    vector<Gdiplus::PointF> vec = m_points.getPoints();
    Gdiplus::PointF* arrPoints = vec.data();
    int cntPoints = vec.size();
    g.FillPolygon(&brush, arrPoints, cntPoints);
    g.DrawLines(&stroke_pen, arrPoints, cntPoints);
}


