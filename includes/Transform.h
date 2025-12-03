#ifndef TRANSFORM_H
#define TRANSFORM_H

#include"myLib.h"

class myTransform {
public:
	virtual ~myTransform(){}
	virtual Gdiplus::Matrix GetMatrix() const = 0;
};

class myTranslate : public myTransform {
	float m_dx, m_dy;
public:
	myTranslate(float dx, float dy) : m_dx(dx), m_dy(dy){}
	Gdiplus::Matrix GetMatrix() const override;
};

class myRotate : public myTransform {
	float m_angle;
public:
	myRotate(float angle) : m_angle(angle) {}
	Gdiplus::Matrix GetMatrix() const override;
};

class myScale : public myTransform{
	float m_sx, m_sy;
public:
	myScale(float sx, float sy) : m_sx(sx), m_sy(sy) {}
	myScale(float d) : m_sx(d), m_sy(d) {}
	Gdiplus::Matrix GetMatrix() const override;
};

class myListTransform {
	vector<myTransform*> m_transforms;
public:
	myListTransform() {}
	~myListTransform() {
		for (auto& x : m_transforms) {
			delete x;
		}
		m_transforms.clear();
	}
	void parse(const char* transformStr);
	Gdiplus::Matrix getFinalMatrix() const;
};



#endif // !TRANSFORM_H
