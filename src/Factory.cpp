#include"Factory.h"

myShape* Factory::getShape(tinyxml2::XMLElement* element) {
	const char* nameTemp = element->Name();
	if (nameTemp == nullptr) {
		return nullptr;
	}
	string name = nameTemp;
	
	if (name == "circle") {
		return new myCircle();
	}
	else if (name == "ellipse") {
		return new myEllipse();
	}
	else if (name == "rect") {
		return new myRect();
	}
	else if (name == "line") {
		return new myLine();
	}
	else if (name == "text") {
		return new myText();
	}
	else if (name == "polygon") {
		return new myPolygon();
	}
	else if (name == "polyline") {
		return new myPolyline();
	}
	else if (name == "g") {
		
	}
	return nullptr;
}