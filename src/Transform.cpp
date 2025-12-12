#include"Transform.h"

void myListTransform::parse(const char* transformStr) {
	if (transformStr == nullptr) return;
	string transform_str = transformStr;
	for (int i = 0; i < transform_str.size(); i++) {
		char c = transform_str[i];
		if (c == '(' || c == ')' || c == ',') transform_str[i] = ' ';
	}
	stringstream ss(transform_str);
	string command;
	while (ss >> command) {
		if (command == "translate") {
			float x, y;
			ss >> x;
			if (ss >> y) {
				m_transforms.push_back(new myTranslate(x, y));
			}
			else {
				ss.clear();
				y = 0.0f;
				m_transforms.push_back(new myTranslate(x, y));
			}
		}
		else if (command == "scale") {
			float x, y;
			ss >> x;
			if (ss >> y) {
				m_transforms.push_back(new myScale(x, y));
			}
			else {
				ss.clear();
				m_transforms.push_back(new myScale(x));
			}
		}
		else if (command == "rotate") {
			float angle;
			ss >> angle;
			m_transforms.push_back(new myRotate(angle));
		}
	}
}