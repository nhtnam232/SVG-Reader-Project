#ifndef PARSER_H
#define PARSER_H

#include"myLib.h"
#include"Factory.h"
#include "ViewBox.h"
#include "Gradient.h"
#include "RadialGradient.h"

class Parser {
	vector<myShape*> m_shapes;
	float svg_width, svg_height;
	ViewBox m_viewBox;
	std::map<std::string, myGradient*> m_gradients;
public:
	Parser();
	~Parser();
	bool loadFile(const string& filePath);
	const vector<myShape*>& getShape() const;
	float getHeightSVG();
	float getWidthSVG();
	ViewBox getViewBox();
	myGradient* getGradient(const std::string& id);
	void parseDefinitions(tinyxml2::XMLElement* parent);
};
extern Parser parser;

#endif 