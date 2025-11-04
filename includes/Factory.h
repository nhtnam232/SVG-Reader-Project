#ifndef FACTORY_H
#define FACTORY_H
#include"myLib.h"
#include"Circle.h"
#include"Ellipse.h"
#include"FilledShape.h"
#include"Group.h"
#include"Line.h"
#include"Polygon.h"
#include"Polyline.h"
#include"Rect.h"
#include"Shape.h"
#include"Text.h"

class Factory
{
	
public:
	
	static Shape* getShape(XMLElement* element);


};


#endif 
