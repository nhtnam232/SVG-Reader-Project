#include "Camera.h"

Camera::Camera(double w, double h)
    : viewportW(w), viewportH(h),
    scale(1.0), angle(0.0),
    center{ w * 0.5, h * 0.5 }
{
}

void Camera::setViewport(double w, double h)
{
    viewportW = w;
    viewportH = h;
}

void Camera::getViewport(double& w, double& h) const
{
    w = viewportW;
    h = viewportH;
}

// screen -> world
Camera::Vec2 Camera::screenToWorld(const Vec2& s) const
{
    Vec2 p{
        (s.x - viewportW * 0.5) / scale,
        (s.y - viewportH * 0.5) / scale
    };

    double rad = -angle * PI / 180.0;
    double c = cos(rad), si = sin(rad);

    Vec2 r{
        c * p.x - si * p.y,
        si * p.x + c * p.y
    };

    return { r.x + center.x, r.y + center.y };
}

// world -> screen
Camera::Vec2 Camera::worldToScreen(const Vec2& w) const
{
    double x = w.x - center.x;
    double y = w.y - center.y;

    x *= scale;
    y *= scale;

    double rad = angle * PI / 180.0;
    double c = cos(rad), si = sin(rad);

    Vec2 r{
        c * x - si * y,
        si * x + c * y
    };

    return {
        r.x + viewportW * 0.5,
        r.y + viewportH * 0.5
    };
}

// ===== ZOOM TÂM MÀN HÌNH =====
void Camera::zoomIn(double factor)
{
    scale = clampDouble(scale * factor, 0.1, 10.0);
}

void Camera::zoomOut(double factor)
{
    scale = clampDouble(scale / factor, 0.1, 10.0);
}

// rotate quanh tâm
void Camera::rotate(double angleDeg)
{
    angle += angleDeg;
    if (angle >= 360.0) angle -= 360.0;
    if (angle < 0.0) angle += 360.0;
}

// reset camera
void Camera::reset()
{
    scale = 1.0;
    angle = 0.0;
    center = { viewportW * 0.5, viewportH * 0.5 };
}
