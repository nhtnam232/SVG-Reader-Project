#ifndef PARSER_H
#define PARSER_H

#include"myLib.h"
#include"Factory.h"
class Parser {
	vector<myShape*> m_shapes;
public:
	Parser();
	~Parser();
	bool loadFile(const string& filePath);
	const vector<myShape*>& getShape() const;
};

#endif 
