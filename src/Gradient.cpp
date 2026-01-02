#include "Gradient.h"

float parseSvgValue(const char* str) {
    if (!str) return 0.0f;

    std::string s(str);
    float value = (float)atof(str); // Lấy phần số

    // Nếu có dấu %, chuyển đổi về hệ 0 -> 1
    if (s.find('%') != std::string::npos) {
        return value / 100.0f;
    }

    // Nếu là số thuần túy, thường SVG coi là pixel hoặc đơn vị hệ tọa độ hiện tại
    return value;
}

void myGradient::parse(tinyxml2::XMLElement* node) {
    if (!node) return;

    const char* id = node->Attribute("id");
    if (id) m_id = id;

    const char* href = node->Attribute("xlink:href");
    if (href) {
        m_parentId = href;
        if (m_parentId[0] == '#') m_parentId.erase(0, 1); // Bỏ dấu #
    }

    const char* units = node->Attribute("gradientUnits");
    m_gradientUnits = units ? units : "objectBoundingBox";

    const char* transformStr = node->Attribute("gradientTransform");
    m_gradientTransform.parse(transformStr);

    tinyxml2::XMLElement* stopNode = node->FirstChildElement("stop");
    while (stopNode) {
        GradientStop stop;

        // Offset: có thể là "0.5" hoặc "50%"
        const char* offsetStr = stopNode->Attribute("offset");
        if (offsetStr) {
            string s = offsetStr;
            if (s.back() == '%') stop.offset = stof(s.substr(0, s.size() - 1)) / 100.0f;
            else stop.offset = stof(s);
        }

        // Màu sắc và Opacity
        const char* stopCol = stopNode->Attribute("stop-color");
        if (stopCol != nullptr)
            stop.m_stop_color = Color(stopCol);
        else stop.m_stop_color = Color("black");

        stop.m_stop_opacity = 1.0f;
        stopNode->QueryFloatAttribute("stop-opacity", &stop.m_stop_opacity);

        m_stops.push_back(stop);
        stopNode = stopNode->NextSiblingElement("stop");
    }
}