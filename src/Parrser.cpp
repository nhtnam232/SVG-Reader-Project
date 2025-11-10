#include"Parser.h"

Parser::Parser() {

}
Parser::~Parser() {
	for (Shape* x : m_shapes) {
		delete x;
	}
	m_shapes.clear();
}
const vector<Shape*>& Parser::getShape() const {
	return m_shapes;
}
bool Parser::loadFile(const string &filePath) {
	XMLDocument doc;
	if (doc.LoadFile(filePath.c_str()) != XML_SUCCESS) {
		return false;
	}
	XMLElement* root = doc.FirstChildElement("svg");
	if (root == nullptr) {
		return false;
	}
	for (XMLElement* node = root->FirstChildElement(); node != nullptr; node = node->NextSiblingElement()) {
		Shape* shape = Factory::getShape(node);
		if (shape != nullptr) {
			shape->parse(node);
			m_shapes.push_back(shape);
		}
	}
	return true;
}
