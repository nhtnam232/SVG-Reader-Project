#include"Polygon.h"
void myPolygon::parse(tinyxml2::XMLElement* node)
{
    if(node == nullptr) {
        std::cerr << "Polygon::parse: NULL node\n";
        return;
    }
    myFilledShape::parse(node);
    m_points = Points(node->Attribute("points"));
}
void myPolygon::draw(Gdiplus::Graphics& g) {
    m_fill.setOpacity(m_fill_opacity);
    m_stroke.setOpacity(m_stroke_opacity);
    vector<Gdiplus::PointF> vec = m_points.getPoints();
    Gdiplus::SolidBrush brush(m_fill.getColor());
    Gdiplus::Pen stroke_pen(m_stroke.getColor(), m_stroke_width);
    Gdiplus::PointF* arrPoints = vec.data();
    int cntPoints = vec.size();

    g.FillPolygon(&brush, arrPoints, cntPoints);
    g.DrawPolygon(&stroke_pen, arrPoints, cntPoints);
}



