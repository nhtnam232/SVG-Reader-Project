#ifndef FACTORY_H
#define FACTORY_H
#include"myLib.h"
#include"Circle.h"
#include"Ellipse.h"
#include"Group.h"
#include"Line.h"
#include"Polygon.h"
#include"Polyline.h"
#include"Rect.h"
#include"Text.h"
class Factory
{
	
public:
	
	static Shape* getShape(tinyxml2::XMLElement* element);


};


#endif 
