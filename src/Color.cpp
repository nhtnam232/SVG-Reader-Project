#include"Color.h"
Color::Color(const string& ColorString) {
	//Add more colors later
	if (ColorString == "none") {
		m_color = Gdiplus::Color::Transparent;
	}
	else if (ColorString == "red") {
		m_color = Gdiplus::Color::Red;
	}
	else if (ColorString == "blue") {
		m_color = Gdiplus::Color::Blue;
	}
	else if (ColorString == "green") {
		m_color = Gdiplus::Color::Green;
	}
	else if (ColorString == "white") {
		m_color = Gdiplus::Color::White;
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
		m_color = Gdiplus::Color(255, (BYTE)r, (BYTE)g, (BYTE)b);
	}
	else {
		m_color = Gdiplus::Color::Black;
	}
}
Gdiplus::Color Color::getColor() const {
	return this->m_color;
}
void Color::setOpacity(float opacity) {
	BYTE alpha = static_cast<BYTE>(opacity * 255.0f);
	BYTE r = m_color.GetR();
	BYTE g = m_color.GetG();
	BYTE b = m_color.GetB();
	m_color = Gdiplus::Color(alpha, r, g, b);
}