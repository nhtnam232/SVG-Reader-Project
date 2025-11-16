#include "Polyline.h"

void Polyline::parse(XMLElement* node)
{
    if(node == nullptr) {
        std::cerr << "Polyline::parse: NULL node\n";
        return;
    }
    FilledShape::parse(node);
    m_points = Points(node->Attribute("points"));
}

void Polyline::draw(sf::RenderWindow& window)
{
    vector<sf::Vector2f> vectorPoints = m_points.getPoints();
    if (vectorPoints.empty()) {
        std::cerr << "Polyline::draw: No points to draw\n";
        return;
    }

    // Thiếu fill trong
    // Thiếu m_stoke_width
    // Xử lý trường hợp đa giác lõm và các điểm tự giao nhau
  
    sf::VertexArray lines(sf::LineStrip, vectorPoints.size());
    for (size_t i = 0; i < vectorPoints.size(); ++i) {
        lines[i].position = vectorPoints[i];
        lines[i].color = m_stroke.getColor();
    }

    window.draw(lines);
}
