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
bool Parser::loadFile(const string &filePath) {
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
		return false;
	}
	tinyxml2::XMLElement* root = doc.FirstChildElement("svg");
	if (root == nullptr) {
		return false;
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
