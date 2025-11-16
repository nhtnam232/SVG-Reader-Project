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


