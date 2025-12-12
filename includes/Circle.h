#ifndef CIRCLE_H
#define CIRCLE_H

#include"myLib.h"
#include"FilledShape.h"

class myCircle : public myFilledShape {
private:
    float m_cx = 0;
    float m_cy = 0;
    float m_rx = 0;
    float m_ry = 0;
    float m_angle = 0; // <<< THUỘC TÍNH MỚI: Góc xoay của ellipse (độ)

public:
    // Khai báo lại các hàm biến đổi
    void transform_scale(float d);
    void transform_scale(float sx, float sy);
    void transform_translate(float dx, float dy);
    void transform_rotate(float angle_degrees); // Hàm xoay mới
    myCircle() : myFilledShape(), m_cx(0), m_cy(0), m_rx(0), m_ry(0) {};
    void parse(tinyxml2::XMLElement*) override;
    void draw(Gdiplus::Graphics& g) override;
};


#endif 
