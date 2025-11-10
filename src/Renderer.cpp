#include"Renderer.h"

Renderer::Renderer(const vector<Shape*>& m_shapes) :m_shapes(m_shapes) {
	m_window.create(sf::VideoMode(800, 600), "SVG READER");
	m_window.setFramerateLimit(60);
}
void Renderer::run() {
	while (m_window.isOpen()) {
		m_window.clear(sf::Color::White);
		handleEvent();
		draw();
		m_window.display();
	}
}
void Renderer::draw() {
	for (Shape* shape : m_shapes) {
		if (shape != nullptr) {
			shape->draw(m_window);
		}
	}
}
void Renderer::handleEvent() {
	sf::Event ev;
	while (m_window.pollEvent(ev)) {
		if (ev.type == sf::Event::Closed) {
			m_window.close();
		}
	}
}
