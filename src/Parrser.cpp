#include"Parser.h"
Parser::Parser() {

}
Parser::~Parser() {
	for (myShape* x : m_shapes) {
		delete x;
	}
	m_shapes.clear();

	for (const std::pair<std::string, myGradient*>& item : m_gradients) {
		myGradient* grad = item.second;
		delete grad;
	}
	m_gradients.clear();
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

	parseDefinitions(root);

	for (tinyxml2::XMLElement* node = root->FirstChildElement(); node != nullptr; node = node->NextSiblingElement()) {
		myShape* shape = Factory::getShape(node);
		if (shape != nullptr) {
			shape->parse(node);
			m_shapes.push_back(shape);
		}
	}
	return true;
}
myGradient* Parser::getGradient(const std::string& id) {
	auto it = m_gradients.find(id);
	if (it != m_gradients.end()) {
		return it->second;
	}
	return nullptr;
}
void Parser::parseDefinitions(tinyxml2::XMLElement* parent) {
	for (tinyxml2::XMLElement* node = parent->FirstChildElement(); node != nullptr; node = node->NextSiblingElement()) {
		string tagName = node->Value();
		myGradient* grad = nullptr;

		if (tagName == "linearGradient") {
			//grad = new myLinearGradient();
		}
		else if (tagName == "radialGradient") {
			grad = new myRadialGradient();
		}
		else if (tagName == "defs") {
			
			parseDefinitions(node);
			continue;
		}

		if (grad) {
			
			grad->parse(node);
			const char* id = node->Attribute("id");
			if (id) {
				m_gradients[id] = grad;
			}
			else {
				delete grad;
			}
		}
	}
}