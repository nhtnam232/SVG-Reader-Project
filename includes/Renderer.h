#ifndef RENDERER_H
#define RENDERER_H
#include"myLib.h"
#include<Shape.h>
class Renderer {
	sf::RenderWindow m_window;
	const vector<Shape*> m_shapes;
public:
	Renderer(const vector<Shape*>&);
	void run();
	void draw();
	void handleEvent();
};


#endif 
