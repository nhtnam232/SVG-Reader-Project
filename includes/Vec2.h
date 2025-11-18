#ifndef VEC2_H
#define VEC2_H

#include "myLib.h"

constexpr double PI = 3.14159265358979323846;

struct Vec2 {
    double x;
    double y;
};

// Khai báo hàm (no static / no inline)
Vec2 make_vec(long x, long y);
Vec2 rotate(const Vec2& v, double angleDeg);
Vec2 rotate_inv(const Vec2& v, double angleDeg);

#endif // VEC2_H
