#include"Color.h"

Color::Color(const string& ColorString) {
	//Add more colors later
	if (ColorString == "none") {
		m_color = sf::Color::Transparent;
	}
	else if (ColorString == "red") {
		m_color = sf::Color::Red;
	}
	else if (ColorString == "blue") {
		m_color = sf::Color::Blue;
	}
	else if (ColorString.find("rgb") == 0) {
		string temp = ColorString.substr(4);
		for (int i = 0; i < temp.size(); i++) {
			if (temp[i] == ',' || temp[i] == ')') {
				temp[i] = ' ';
			}
		}
		stringstream ss(temp);
		int r, g, b;
		ss >> r >> g >> b;
		m_color = sf::Color(r, g, b);
	}
	else {
		m_color = sf::Color::Black;
	}
}
sf::Color Color::getColor() const {
	return this->m_color;
}
void Color::setOpacity(float opacity) {
	m_color.a = static_cast<sf::Uint8>(opacity * 255);
}