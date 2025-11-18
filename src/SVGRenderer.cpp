// SVGRenderer.cpp
#include "SVGRenderer.h"

void SVGRenderer::applyTransform(const Camera& cam, Gdiplus::Graphics& graphics) {
    graphics.TranslateTransform((Gdiplus::REAL)cam.getCenter().x,
        (Gdiplus::REAL)cam.getCenter().y);
    graphics.RotateTransform((Gdiplus::REAL)cam.getAngle());
    graphics.ScaleTransform((Gdiplus::REAL)cam.getScale(), (Gdiplus::REAL)cam.getScale());
    graphics.TranslateTransform(-(Gdiplus::REAL)cam.getCenter().x,
        -(Gdiplus::REAL)cam.getCenter().y);
}

void SVGRenderer::render(const std::vector<myShape*>& shapes, Gdiplus::Graphics& graphics) {
    // hiện tại không vẽ gì, chỉ placeholder
    for (myShape* s : shapes) {
        if (s) s->draw(graphics);
    }
}

