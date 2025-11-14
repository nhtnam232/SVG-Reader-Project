#include "Polygon.h"

void Polygon::parse(XMLElement* node)
{
    if(node == nullptr) {
        std::cerr << "Polygon::parse: NULL node\n";
        return;
    }
    FilledShape::parse(node);
    m_points = Points(node->Attribute("points"));
}

void Polygon::draw(sf::RenderWindow& window)
{
    vector <sf::Vector2f> vectorPoints = m_points.getPoints();
    if(vectorPoints.empty()) {
        std::cerr << "Polygon::draw: No points to draw\n";
        return;
    }
    
    sf::ConvexShape polygon;
    polygon.setPointCount(vectorPoints.size());
    for(int i = 0; i < vectorPoints.size(); i++)
        polygon.setPoint(i,vectorPoints[i]);

    m_fill.setOpacity(m_fill_opacity);
    polygon.setFillColor(m_fill.getColor());
    m_stroke.setOpacity(m_stroke_opacity);
    polygon.setOutlineColor(m_stroke.getColor());
    polygon.setOutlineThickness(m_stroke_width);

    window.draw(polygon);
}
