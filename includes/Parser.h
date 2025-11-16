#ifndef PARSER_H
#define PARSER_H

#include"myLib.h"
#include"Factory.h"
class Parser {
	vector<Shape*> m_shapes;
public:
	Parser();
	~Parser();
	bool loadFile(const string& filePath);
	const vector<Shape*>& getShape() const;
};

#endif 
