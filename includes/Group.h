#ifndef GROUP_H
#define GROUP_H

#include"myLib.h"
#include"FilledShape.h"
#include"Factory.h"
class myGroup : public myFilledShape {
	vector<myShape*> m_children;
public:
	~myGroup();
	myGroup() : myFilledShape() {}
	void parse(tinyxml2::XMLElement*) override;
	void draw(Gdiplus::Graphics&) override;
	const vector<myShape*> getChildren();
};

#endif 
