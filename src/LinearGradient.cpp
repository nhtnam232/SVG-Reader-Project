#include "LinearGradient.h"

void myLinearGradient::parse(tinyxml2::XMLElement* node) {
    myGradient::parse(node);
    const char* x1 = node->Attribute("x1");
    if (x1 != nullptr) {
        m_x1 = parseSvgValue(x1);
    }
    const char* y1 = node->Attribute("y1");
    if (y1 != nullptr) {
        m_y1 = parseSvgValue(y1);
    }
    const char* x2 = node->Attribute("x2");
    if (x2 != nullptr) {
        m_x2 = parseSvgValue(x2);
    }
    const char* y2 = node->Attribute("y2");
    if (y2 != nullptr) {
        m_y2 = parseSvgValue(y2);
    }
}

Gdiplus::Brush* myLinearGradient::createBrush(const Gdiplus::RectF& objectBounds) {
    if (m_stops.empty() && !m_parentId.empty()) {
        myGradient* parent = parser.getGradient(m_parentId);
        if (parent != nullptr) {
            m_stops = parent->getStops();
        }
    }
    if (m_stops.empty()) {
        return nullptr;
    }
    sort(m_stops.begin(), m_stops.end(), [](const GradientStop& a, const GradientStop& b) {
            return a.offset < b.offset;
        });
    if (m_stops.front().offset > 0.0f) {
        GradientStop s = m_stops.front();
        s.offset = 0.0f;
        m_stops.insert(m_stops.begin(), s);
    }
    if (m_stops.back().offset < 1.0f) {
        GradientStop s = m_stops.back();
        s.offset = 1.0f;
        m_stops.push_back(s);
    }
    Gdiplus::PointF start, end;
    if (m_gradientUnits == "userSpaceOnUse") {
        start = Gdiplus::PointF(m_x1, m_y1);
        end = Gdiplus::PointF(m_x2, m_y2);
    }
    else {
        start = Gdiplus::PointF(objectBounds.X + m_x1 * objectBounds.Width,
            objectBounds.Y + m_y1 * objectBounds.Height);
        end = Gdiplus::PointF(objectBounds.X + m_x2 * objectBounds.Width,
            objectBounds.Y + m_y2 * objectBounds.Height);
    }
    if (abs(start.X - end.X) < 0.001f && abs(start.Y - end.Y) < 0.001f) {
        Color onlyColor = m_stops.front().m_stop_color;
        onlyColor.setOpacity(m_stops.front().m_stop_opacity);
        return new Gdiplus::SolidBrush(onlyColor.getColor());
    }
    Color beginColor = m_stops.front().m_stop_color;
    beginColor.setOpacity(m_stops.front().m_stop_opacity);
    Color endColor = m_stops.back().m_stop_color;
    endColor.setOpacity(m_stops.back().m_stop_opacity);
    Gdiplus::LinearGradientBrush* brush = new Gdiplus::LinearGradientBrush(
        start,
        end,
        beginColor.getColor(),
        endColor.getColor()
    );
    if (brush->GetLastStatus() != Gdiplus::Ok) {
        delete brush;
        return nullptr;
    }
    if (m_stops.size() > 2) {
        int count = m_stops.size();
        Gdiplus::Color* colors = new Gdiplus::Color[count];
        Gdiplus::REAL* positions = new Gdiplus::REAL[count];

        for (int i = 0; i < count; i++) {
            m_stops[i].m_stop_color.setOpacity(m_stops[i].m_stop_opacity);
            colors[i] = m_stops[i].m_stop_color.getColor();
            positions[i] = max(0.0f, std::min(1.0f, m_stops[i].offset));
        }
        brush->SetInterpolationColors(colors, positions, count);
        delete[] colors;
        delete[] positions;
    }
    brush->MultiplyTransform(m_gradientTransform.getFinalMatrix());
    return brush;
}