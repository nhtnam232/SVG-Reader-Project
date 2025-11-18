#include "Vec2.h"

// Định nghĩa hàm
Vec2 make_vec(long x, long y) {
    Vec2 v;
    v.x = static_cast<double>(x);
    v.y = static_cast<double>(y);
    return v;
}

Vec2 rotate(const Vec2& v, double angleDeg) {
    double a = angleDeg * PI / 180.0;
    double c = std::cos(a);
    double s = std::sin(a);
    return Vec2{ c * v.x - s * v.y, s * v.x + c * v.y };
}

Vec2 rotate_inv(const Vec2& v, double angleDeg) {
    return rotate(v, -angleDeg);
}
