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

Gdiplus::Matrix* myTranslate::GetMatrix() const {
	return new Gdiplus::Matrix(1.0f, 0.0f, 0.0f, 1.0f, m_dx, m_dy);
}

Gdiplus::Matrix* myRotate::GetMatrix() const {
	float angleRad = m_angle * PI / 180.0f; // Deegre to radian
	float cosAngle = cos(angleRad);
	float sinAngle = sin(angleRad);
	return new Gdiplus::Matrix(cosAngle, sinAngle, -sinAngle, cosAngle, 0.0f, 0.0f);
}

Gdiplus::Matrix* myScale::GetMatrix() const {
	return new Gdiplus::Matrix(m_sx, 0.0f, 0.0f, m_sy, 0.0f, 0.0f);
}

Gdiplus::Matrix* myListTransform::getFinalMatrix() const {
	Gdiplus::Matrix* finalMatrix = new Gdiplus::Matrix();

	for (int i = m_transforms.size() - 1; i >= 0; --i) {
		Gdiplus::Matrix* tempMtx = m_transforms[i]->GetMatrix();
		finalMatrix->Multiply(tempMtx, Gdiplus::MatrixOrderPrepend);
		delete tempMtx;
	}

	return finalMatrix;
}