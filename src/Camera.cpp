#include "Camera.h"


Camera::Camera(double viewportW_, double viewportH_)
    : viewportW(viewportW_), viewportH(viewportH_),
    scale(1.0), angle(0.0),
    center{ viewportW_ / 2.0, viewportH_ / 2.0 } {
}

void Camera::setViewport(double w, double h) {
    viewportW = w; viewportH = h;
    center = { viewportW * 0.5, viewportH * 0.5 };
}

void Camera::getViewport(double& w, double& h) const {
    w = viewportW; h = viewportH;
}

// screen -> world
Camera::Vec2 Camera::screenToWorld(const Vec2& screenPt) const {
    // Offset từ viewport center
    Vec2 offset = { screenPt.x - viewportW * 0.5,
                    screenPt.y - viewportH * 0.5 };

    // undo scale
    offset.x /= scale;
    offset.y /= scale;

    // undo rotation
    double rad = -angle * PI / 180.0;
    double cosA = std::cos(rad), sinA = std::sin(rad);
    Vec2 rotated = { cosA * offset.x - sinA * offset.y,
                     sinA * offset.x + cosA * offset.y };

    // add center
    return { rotated.x + center.x, rotated.y + center.y };
}

// world -> screen
Camera::Vec2 Camera::worldToScreen(const Vec2& worldPt) const {
    double wx = worldPt.x - center.x;
    double wy = worldPt.y - center.y;

    // scale
    wx *= scale;
    wy *= scale;

    // rotate
    double rad = angle * PI / 180.0;
    double cosA = std::cos(rad), sinA = std::sin(rad);
    double sx = wx * cosA - wy * sinA;
    double sy = wx * sinA + wy * cosA;

    // translate to viewport center
    sx += viewportW * 0.5;
    sy += viewportH * 0.5;

    return Vec2{ sx, sy };
}

// Zoom quanh điểm screenPt (con trỏ) cố định
void Camera::zoomAt(double factor, const Vec2& cursorScreenPos) {
    // Vị trí thế giới trước zoom
    Vec2 worldBefore = screenToWorld(cursorScreenPos);

    // Thay đổi scale
    scale = clampDouble(scale * factor, 0.1, 10.0);

    // Vị trí thế giới sau zoom
    Vec2 worldAfter = screenToWorld(cursorScreenPos);

    // Dịch center để điểm dưới con trỏ giữ cố định
    center.x += (worldBefore.x - worldAfter.x);
    center.y += (worldBefore.y - worldAfter.y);
}

// Zoom in/out
void Camera::zoomIn(double factor, const Vec2& cursorScreenPos) {
    zoomAt(factor, cursorScreenPos);
}
void Camera::zoomOut(double factor, const Vec2& cursorScreenPos) {
    zoomAt(1.0 / factor, cursorScreenPos);
}

// Rotate quanh center
void Camera::rotate(double angleDeg) {
    angle += angleDeg;
    angle = std::fmod(angle, 360.0);
    if (angle < 0) angle += 360.0;
}

// Reset camera
void Camera::reset() {
    scale = 1.0;
    angle = 0.0;
    center = { viewportW * 0.5, viewportH * 0.5 };
}
