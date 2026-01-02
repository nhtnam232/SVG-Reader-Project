#ifndef _GRADIENT_H_
#define _GRADIENT_H_

#include "Shape.h"
#include "Color.h"
#include "Transform.h"

struct GradientStop {
    float offset;
    Color m_stop_color;
    float m_stop_opacity;
};

class myGradient {
protected:
    std::string m_id;
    std::string m_parentId;
    std::vector<GradientStop> m_stops;
    myListTransform m_gradientTransform;
    std::string m_gradientUnits;

public:
    virtual ~myGradient() {}
    virtual Gdiplus::Brush* createBrush(const Gdiplus::RectF& objectBounds) = 0;
    virtual void parse(tinyxml2::XMLElement* node);

    void setParentId(const std::string& id) { m_parentId = id; }
    const std::string& getParentId() const { return m_parentId; }
    bool hasParent() const { return !m_parentId.empty(); }
    const std::vector<GradientStop>& getStops() const { return m_stops; }
};

float parseSvgValue(const char* str);


#endif // _GRADIENT_H_