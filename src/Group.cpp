#include"Group.h"


myGroup::~myGroup() {
	for (myShape* x : m_children) {
		delete x;
	}
	m_children.clear();
}
void myGroup::parse(tinyxml2::XMLElement* node) {
	myFilledShape::parse(node);

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
			child->parse(subNode);
			m_children.push_back(child);
		}
	}
}
void myGroup::draw(Gdiplus::Graphics& g) {
	Gdiplus::GraphicsState state = g.Save();
	for (myShape* child : m_children) {
		child->draw(g);
	}
	g.Restore(state);
}

