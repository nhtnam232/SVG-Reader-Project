#ifndef PARSER_H
#define PARSER_H

#include"myLib.h"
#include"Factory.h"
#include "ViewBox.h"
class Parser {
	vector<myShape*> m_shapes;
	float svg_width, svg_height;
	ViewBox m_viewBox;
public:
	Parser();
	~Parser();
	bool loadFile(const string& filePath);
	const vector<myShape*>& getShape() const;
	float getHeightSVG();
	float getWidthSVG();
	ViewBox getViewBox();
};

#endif 
