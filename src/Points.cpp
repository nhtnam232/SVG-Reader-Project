#include"Points.h"

Points::Points(const char* pointsString) {
	if (pointsString == nullptr) {
		return;
	}
	string pointsStr = pointsString;
	for (int i = 0; i < pointsStr.size(); i++) {
		if (pointsStr[i] == ',') {
			pointsStr[i] = ' ';
		}
	}
	stringstream ss(pointsStr);
	float x, y;
	while (ss >> x >> y) {
		m_points.push_back(sf::Vector2f(x, y));
	}
}
const vector<sf::Vector2f>& Points::getPoints() const {
	return this->m_points;
}