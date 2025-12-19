#include"Parser.h"
Parser::Parser() {

}
Parser::~Parser() {
	for (myShape* x : m_shapes) {
		delete x;
	}
	m_shapes.clear();
}
const vector<myShape*>& Parser::getShape() const {
	return m_shapes;
}
float Parser::getHeightSVG() {
	return svg_height;
}
float Parser::getWidthSVG() {
	return svg_width;
}
ViewBox Parser::getViewBox() {
	return m_viewBox;
}
bool Parser::loadFile(const string &filePath) {
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
		return false;
	}
	tinyxml2::XMLElement* root = doc.FirstChildElement("svg");
	if (root == nullptr) {
		return false;
	}
	svg_width = 0;
	svg_height = 0;
	root->QueryFloatAttribute("width", &svg_width);
	root->QueryFloatAttribute("height", &svg_height);
	const char* viewBoxStr = root->Attribute("viewBox");
	if (viewBoxStr) {
		m_viewBox.parse(viewBoxStr);
	}
	else {
		m_viewBox.setViewBox(0, 0, svg_width, svg_height);
	}

	for (tinyxml2::XMLElement* node = root->FirstChildElement(); node != nullptr; node = node->NextSiblingElement()) {
		myShape* shape = Factory::getShape(node);
		if (shape != nullptr) {
			shape->parse(node);
			m_shapes.push_back(shape);
		}
	}
	return true;
}
