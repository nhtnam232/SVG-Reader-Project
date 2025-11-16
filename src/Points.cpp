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
		m_points.push_back(Gdiplus::PointF(x, y));
	}
}
const vector<Gdiplus::PointF>& Points::getPoints() const {
	return this->m_points;
}