#ifndef CAMERA_H
#define CAMERA_H

#include "myLib.h"

class Camera {
public:
    struct Vec2 {
        double x;
        double y;
    };

    Camera(double viewportW = 1920.0, double viewportH = 1080.0);

    // viewport
    void setViewport(double w, double h);
    void getViewport(double& w, double& h) const;

    // transform
    Vec2 screenToWorld(const Vec2& screenPt) const;
    Vec2 worldToScreen(const Vec2& worldPt) const;

    // camera control (ZOOM TÂM MÀN HÌNH)
    void zoomIn(double factor);
    void zoomOut(double factor);
    void rotate(double angleDeg);
    void reset();

    // accessors
    double getScale() const { return scale; }
    double getAngle() const { return angle; }
    Vec2 getCenter() const { return center; }
    void setCenter(const Vec2& c) { center = c; }

private:
    double viewportW, viewportH;
    double scale;
    double angle;
    Vec2 center;

    static constexpr double PI = 3.14159265358979323846;
    static double clampDouble(double v, double lo, double hi) {
        return (v < lo) ? lo : ((v > hi) ? hi : v);
    }
};

#endif // CAMERA_H
