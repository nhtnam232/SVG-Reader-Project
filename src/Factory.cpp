#include"Factory.h"

Shape* Factory::getShape(XMLElement* element) {
	const char* nameTemp = element->Name();
	if (nameTemp == nullptr) {
		return nullptr;
	}
	string name = nameTemp;
	
	if (name == "circle") {
		return new Circle();
	}
	else if (name == "ellipse") {
		return new Ellipse();
	}
	else if (name == "rect") {
		return new Rect();
	}
	else if (name == "line") {
		return new Line();
	}
	else if (name == "text") {
		return new Text();
	}
	else if (name == "polygon") {
		return new Polygon();
	}
	else if (name == "polyline") {
		return new Polyline();
	}
	else if (name == "g") {
		
	}
	return nullptr;
}