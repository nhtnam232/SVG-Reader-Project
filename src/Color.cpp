#include"Color.h"

map<string, Gdiplus::Color> Color::color_map;
void Color::loadColor(const string& fileName) {
	ifstream in(fileName);
	if (!in.is_open()) {
		color_map["black"] = Gdiplus::Color::Black;
		color_map["white"] = Gdiplus::Color::White;
		color_map["none"] = Gdiplus::Color::Transparent;
		return;
	}
	color_map["none"] = Gdiplus::Color::Transparent;
	string name, hex;
	string line;
	while (getline(in, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		if (!(ss >> name >> hex)) continue;
		transform(name.begin(), name.end(),name.begin(), ::tolower);
		if (name == "none") {
			color_map[name] = Gdiplus::Color::Transparent;
		}
		else {
			if (hex.empty()) continue;
			string hexCode = hex;
			string ans = "";
			if (hexCode.length() == 3) {
				ans += string(2, hexCode[0]);
				ans += string(2, hexCode[1]);
				ans += string(2, hexCode[2]);
			}
			else ans = hexCode;
			if (ans.length() == 6) {
				int rgbValue = stoi(ans, nullptr, 16);
				int r = (rgbValue >> 16) & 0xFF;
				int g = (rgbValue >> 8) & 0xFF;
				int b = (rgbValue) & 0xFF;
				color_map[name] = Gdiplus::Color(255, (BYTE)r, (BYTE)g, (BYTE)b);
			}
			else {
				color_map[name] = Gdiplus::Color::Black;
			}
		}
	}
}
Color::Color(const string& ColorString) {
	if (color_map.empty()) {
		Color::loadColor("resource\\ColorResource.txt");
	}
	string temp = ColorString;

	if (ColorString.find("rgb") == 0) {
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
	else if (ColorString[0] == '#') {
		string hexCode = ColorString.substr(1);
		string ans = "";
		if (hexCode.length() == 3) {
			ans += string(2, hexCode[0]);
			ans += string(2, hexCode[1]);
			ans += string(2, hexCode[2]);
		}
		else ans = hexCode;
		if (ans.length() == 6) {
			int rgbValue = stoi(ans, nullptr, 16);
			int r = (rgbValue >> 16) & 0xFF;
			int g = (rgbValue >> 8) & 0xFF;
			int b = (rgbValue) & 0xFF;
			m_color = Gdiplus::Color(255, (BYTE)r, (BYTE)g, (BYTE)b);
		}
		else {
			m_color = Gdiplus::Color::Black;
		}
	}
	else {
		transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
		if (color_map.find(temp) != color_map.end()) {
			m_color = color_map[temp];
		}
		else {
			m_color = Gdiplus::Color::Black;
		}
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