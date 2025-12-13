#include"Group.h"


myGroup::~myGroup() {
	for (myShape* x : m_children) {
		delete x;
	}
	m_children.clear();
}
void myGroup::parse(tinyxml2::XMLElement* node) {
	myFilledShape::parse(node);
	float fontSize = 0;
	node->QueryFloatAttribute("font-size", &fontSize);
	const char* fontFamilyAttr = node->Attribute("font-family");
	const char* fontStyleAttr = node->Attribute("font-style");
	const char* fontWeightAttr = node->Attribute("font-weight");
	for (tinyxml2::XMLElement* subNode = node->FirstChildElement(); subNode != nullptr; subNode = subNode->NextSiblingElement()) {
		myShape* child = Factory::getShape(subNode);
		if (child != nullptr) {
			child->setStroke(this->getStroke());
			child->setStrokeOpacity(this->getStrokeOpacity());
			child->setStrokeWidth(this->getStrokeWidth());
			if (child->isFilledShape()) {
				myFilledShape* childTemp = static_cast<myFilledShape*>(child);
				childTemp->setFill(this->getFill());
				childTemp->setFillOpacity(this->getFillOpacity());
			}
			myText* childText = dynamic_cast<myText*> (child);
			if (childText != nullptr) {
				childText->setFontFamily(fontFamilyAttr);
				childText->setFontSize(fontSize);
				childText->setFontStyle(fontStyleAttr, fontWeightAttr);
			}
			child->parse(subNode);
			m_children.push_back(child);
		}
	}
}
void myGroup::draw(Gdiplus::Graphics& g) {

	Gdiplus::Matrix originalMatrix;
	g.GetTransform(&originalMatrix);
	Gdiplus::Matrix* transformMatrix = m_transforms.getFinalMatrix();
	if (transformMatrix != nullptr) g.MultiplyTransform(transformMatrix);
	for (myShape* child : m_children) {
		child->draw(g);
	}
	g.SetTransform(&originalMatrix);
}

