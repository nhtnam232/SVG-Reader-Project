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

        // camera control
        void zoomAt(double factor, const Vec2& cursorScreen); // zoom giữ con trỏ
        void zoomIn(double factor, const Vec2& cursorScreen);
        void zoomOut(double factor, const Vec2& cursorScreen);

        void rotate(double angleDeg); // degrees, relative
        void reset();

        // accessors
        double getScale() const { return scale; }
        double getAngle() const { return angle; }
        Vec2 getCenter() const { return center; }
        void setCenter(const Vec2& c) { center = c; }

    private:
        double viewportW, viewportH;
        double scale;    // 1.0 = 100%
        double angle;    // degrees, positive = rotate clockwise for world->screen
        Vec2 center;     // world coordinates of viewport center

        static constexpr double PI = 3.14159265358979323846;
        static double degToRad(double d) { return d * PI / 180.0; }
        static double clampDouble(double v, double lo, double hi) {
            return (v < lo) ? lo : ((v > hi) ? hi : v);
        }
    };

    #endif // CAMERA_H
