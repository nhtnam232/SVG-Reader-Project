#ifndef PATH_H
#define PATH_H

#include "myLib.h"
#include "FilledShape.h"

class myPath : public myFilledShape {
	Gdiplus::GraphicsPath* m_path;
public:
	myPath() : myFilledShape(), m_path(new Gdiplus::GraphicsPath()) {};
	~myPath() { delete m_path; }
	void parse(tinyxml2::XMLElement* node) override;
	void draw(Gdiplus::Graphics& g) override;
};


#endif // !PATH_H
