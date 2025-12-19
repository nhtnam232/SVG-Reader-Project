#ifndef VIEWBOX_H
#define VIEWBOX_H
#include<myLib.h>
#include "ViewBox.h"

class ViewBox {
	float x, y, w, h;
public:
	ViewBox() : x(0), y(0), w(0), h(0){}
	ViewBox(float xNew, float yNew, float wNew, float hNew) : x(xNew), y(yNew), w(wNew), h(hNew) {}
	void setViewBox(float x, float y, float w, float h);
	bool hasViewBox = false;
	void parse(const char*);
	Gdiplus::Matrix* getTransformMatrix(float screenW, float screenH);

	

};


#endif 
