#ifndef ELLIPSE_H
#define ELLIPSE_H	

#include"myLib.h"
#include"FilledShape.h"

class myEllipse : public myFilledShape {
private:
    float m_cx = 0;    // Tọa độ tâm X
    float m_cy = 0;    // Tọa độ tâm Y
    float m_rx = 0;    // Bán trục X
    float m_ry = 0;    // Bán trục Y
    float m_angle = 0; // Góc xoay của ellipse (bằng độ)

public:
    // Hàm khởi tạo (nếu cần)
    myEllipse() = default;

    // Các hàm chính
    void parse(tinyxml2::XMLElement* node);
    void draw(Gdiplus::Graphics& g);

    // Các hàm biến đổi (Transformations)
    // Scale đồng đều (Uniform scale)
    void transform_scale(float d);
    // Scale không đồng đều (Non-uniform scale)
    void transform_scale(float sx, float sy);
    // Dịch chuyển
    void transform_translate(float dx, float dy);
    // Xoay (quanh tâm)
    void transform_rotate(float angle_degrees);
};

#endif 
