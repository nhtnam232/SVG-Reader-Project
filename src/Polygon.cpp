#include"Polygon.h"
void Polygon::parse(tinyxml2::XMLElement* node)
{
    if(node == nullptr) {
        std::cerr << "Polygon::parse: NULL node\n";
        return;
    }
    FilledShape::parse(node);
    m_points = Points(node->Attribute("points"));
}


