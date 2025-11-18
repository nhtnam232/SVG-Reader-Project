// SVGRenderer.h
#ifndef SVGRENDERER_H
#define SVGRENDERER_H

#include "myLib.h"
#include "Shape.h"
#include "Camera.h"

class SVGRenderer {
public:
    void render(const std::vector<myShape*>& shapes, Gdiplus::Graphics& graphics);
    void applyTransform(const Camera& cam, Gdiplus::Graphics& graphics);
};

#endif // SVGRENDERER_H
