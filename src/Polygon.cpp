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


