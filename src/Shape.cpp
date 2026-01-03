#include"Shape.h"
void myShape::parse(tinyxml2::XMLElement* node) {
	const char* styleStr = node->Attribute("style");
	if (styleStr != nullptr) {
		applyStyleAttributes(parseStyle(styleStr));
	}
	const char* stroke_value = node->Attribute("stroke");
	if (stroke_value != nullptr) {
		string strokeStr = stroke_value;
		if (Color::isUrl(stroke_value))
			this->m_stroke_gradient_id = Color::extractId(strokeStr);
		else {
			m_stroke = Color(stroke_value);
			m_stroke_gradient_id = "";
		}
		m_stroke_width = 1.0;
	}
	node->QueryFloatAttribute("stroke-width", &m_stroke_width);
	node->QueryFloatAttribute("stroke-opacity", &m_stroke_opacity);
	const char* transformStr = node->Attribute("transform");
	m_transforms.parse(transformStr);
}

const map<string, string> myShape::parseStyle(const char* styleStr) {
	string sstyle = styleStr;
	istringstream stream(sstyle);
	string pair;
	map<string, string> attributes;
	while (getline(stream, pair, ';')) {
		if (pair.empty()) {
			continue;
		}
		size_t colonPos = pair.find(':');
		const std::string whitespace = " \t\n\r\f\v";
		if (colonPos != std::string::npos) {
			string key = pair.substr(0, colonPos);
			// trim space
			size_t keyStart = key.find_first_not_of(whitespace);
			size_t keyEnd = key.find_last_not_of(whitespace);
			key = key.substr(keyStart, keyEnd - keyStart + 1);

			string value = pair.substr(colonPos + 1);
			// trim space
			size_t valueStart = value.find_first_not_of(whitespace);
			size_t valueEnd = value.find_last_not_of(whitespace);
			value = value.substr(valueStart, valueEnd - valueStart + 1);

			attributes.emplace(key, value);
		}
	}
	return attributes;
}

void myShape::applyStyleAttributes(const map<string, string>& attributes) {
	for (auto& pair : attributes) {
		if (pair.first == "stroke") {
			if (Color::isUrl(pair.second)) {
				m_stroke_gradient_id = Color::extractId(pair.second.c_str());
			}
			else {
				m_stroke = Color(pair.second);
			}
			m_stroke_width = 1.0;
		}
		else if (pair.first == "stroke-width") {
			m_stroke_width = stof(pair.second);
		}
		else if (pair.first == "stroke-opacity") {
			m_stroke_opacity = stof(pair.second);
		}
		else if (pair.first == "transform") {
			m_transforms.parse(pair.second.c_str());
		}
	}
}

Color myShape::getStroke() {
	return m_stroke;
}
float myShape::getStrokeWidth() {
	return m_stroke_width;
}
float myShape::getStrokeOpacity() {
	return m_stroke_opacity;
}
void myShape::setStroke(Color other) {
	m_stroke = other;
}
void myShape::setStrokeWidth(float other) {
	m_stroke_width = other;
}
void myShape::setStrokeOpacity(float other) {
	m_stroke_opacity = other;
}

