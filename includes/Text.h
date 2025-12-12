#ifndef TEXT_H
#define TEXT_H
#include"myLib.h"
#include"FilledShape.h"

class myText : public myFilledShape {
private:
    float m_x, m_y, m_font_size;
    std::string m_text;

    // Bổ sung các thuộc tính cho Biến đổi
    float m_angle = 0; // Góc xoay của văn bản (bằng độ)
    float m_scale_x = 1.0f; // Hệ số scale hiện tại theo X
    float m_scale_y = 1.0f; // Hệ số scale hiện tại theo Y

public:
    myText() : myFilledShape(), m_x(0), m_y(0), m_font_size(12), m_text(""), m_angle(0), m_scale_x(1.0f), m_scale_y(1.0f) {}

    void parse(tinyxml2::XMLElement*) override;
    void draw(Gdiplus::Graphics& g) override;

    // Các hàm biến đổi (Transformations)
    void transform_scale(float d);
    void transform_scale(float sx, float sy);
    void transform_translate(float dx, float dy);
    void transform_rotate(float angle_degrees);
};


#endif 
