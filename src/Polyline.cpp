#include"Polyline.h"

void Polyline::parse(tinyxml2::XMLElement* node)
{
    if(node == nullptr) {
        std::cerr << "Polyline::parse: NULL node\n";
        return;
    }
    FilledShape::parse(node);
    m_points = Points(node->Attribute("points"));
}


