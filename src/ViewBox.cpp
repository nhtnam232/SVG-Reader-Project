#include"ViewBox.h"

void ViewBox::setViewBox(float x, float y, float w, float h) {
	this->x = x; this->y = y;
	this->w = w; this->h = h;
	if (w > 0 && h > 0) {
		this->hasViewBox = true;
	}
	else {
		this->hasViewBox = false;
	}
}
void ViewBox::parse(const char* viewBoxStr) {
	if (viewBoxStr == nullptr) {
		hasViewBox = false;
		return;
	}
	string viewBoxString = viewBoxStr;
	for (int i = 0; i < viewBoxString.size(); i++) {
		if (viewBoxString[i] == ',') viewBoxString[i] = ' ';
	}
	stringstream ss(viewBoxString);
	if (ss >> x >> y >> w >> h) {
		if (w > 0 && h > 0) {
			hasViewBox = true;
		}
	}
}
Gdiplus::Matrix* ViewBox::getTransformMatrix(float screenW, float screenH) {
	if (!hasViewBox) {
		return new Gdiplus::Matrix();
	}
	float scaleX = screenW / w; float scaleY = screenH / h;
	float scale = min(scaleX, scaleY);

	float offsetX = (screenW - w * scale) / 2.0f;
	float offsetY = (screenH - h * scale) / 2.0f;

	Gdiplus::Matrix* matrix = new Gdiplus::Matrix();
	matrix->Translate(-x, -y, Gdiplus::MatrixOrderAppend);
	matrix->Scale(scale, scale, Gdiplus::MatrixOrderAppend);
	matrix->Translate(offsetX, offsetY, Gdiplus::MatrixOrderAppend);
	return matrix;
}