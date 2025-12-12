#ifndef RECT_H
#define RECT_H

#include"myLib.h"
#include"FilledShape.h"

class myRect : public myFilledShape {
private:
    float m_x, m_y, m_width, m_height;
    // Bổ sung các thuộc tính cần thiết cho Biến đổi
    float m_cx = 0;    // Tọa độ tâm X
    float m_cy = 0;    // Tọa độ tâm Y
    float m_angle = 0; // Góc xoay của hình chữ nhật (bằng độ)
public:
    myRect() : myFilledShape(), m_x(0), m_y(0), m_width(0), m_height(0), m_cx(0), m_cy(0), m_angle(0) {}

    void parse(tinyxml2::XMLElement*) override;
    void draw(Gdiplus::Graphics& g) override;

    // Các hàm biến đổi
    void transform_scale(float d);
    void transform_scale(float sx, float sy);
    void transform_translate(float dx, float dy);
    void transform_rotate(float angle_degrees);
};



#endif 
