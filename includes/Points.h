#ifndef POINTS_H
#define POINTS_H
#include"myLib.h"


class Points {
	vector<Gdiplus::PointF> m_points;
public:
	Points() {}
	Points(const char*);
	const vector<Gdiplus::PointF>& getPoints() const;
};




#endif // !POINTS_H
