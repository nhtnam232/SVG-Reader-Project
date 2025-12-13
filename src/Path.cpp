#include "Path.h"

void myPath::parse(tinyxml2::XMLElement* node) {
	myFilledShape::parse(node);
	const char* d = node->Attribute("d");
	if (d == nullptr) return;
	string pathStr = d;
	for (int i = 0; i < pathStr.size(); i++) {
		if (pathStr[i] == ',') pathStr[i] = ' ';
	}
	stringstream ss(pathStr);
	char c;
	float startX = 0.0f, startY = 0.0f, endX = 0.0f, endY = 0.0f;
	while (ss >> c) {
		if (c == 'M' || c == 'm') {
			float x, y;
			char nextChr;
			int cnt = 0;
			while (true) {
				nextChr = ss.peek();
				while (nextChr == ' ' || nextChr == ',' || nextChr == '\t' || nextChr == '\n') {
					ss.ignore();
					nextChr = ss.peek();
				}
				if (!isdigit(nextChr) && nextChr != '-' && nextChr != '.') {
					break;
				}
				if (ss >> x >> y) {
					float targetX, targetY;
					if (c == 'M') {
						targetX = x;
						targetY = y;
					}
					else if (c == 'm') {
						targetX = endX + x;
						targetY = endY + y;
					}
					if (cnt > 0) {
						m_path->AddLine(endX, endY, targetX, targetY);
					}
					else {
						m_path->StartFigure();
						startX = targetX;
						startY = targetY;
					}
					endX = targetX;
					endY = targetY;
					cnt += 1;
				}
				else {
					break;
				}
			}
		}
		else if (c == 'L' || c == 'l') {
			float x, y;
			char nextChr;
			while (true) {
				nextChr = ss.peek();
				while (nextChr == ' ' || nextChr == ',' || nextChr == '\t' || nextChr == '\n') {
					ss.ignore();
					nextChr = ss.peek();
				}
				if (!isdigit(nextChr) && nextChr != '-' && nextChr != '.') {
					break;
				}
				if (ss >> x >> y) {
					float targetX, targetY;
					if (c == 'L') {
						targetX = x;
						targetY = y;
					}
					else if (c == 'l') {
						targetX = endX + x;
						targetY = endY + y;
					}
					m_path->AddLine(endX, endY, targetX, targetY);
					endX = targetX;
					endY = targetY;
				}
				else {
					break;
				}
			}
		}
		else if (c == 'H' || c == 'h') {
			float x;
			char nextChr;
			while (true) {
				nextChr = ss.peek();
				while (nextChr == ' ' || nextChr == ',' || nextChr == '\t' || nextChr == '\n') {
					ss.ignore();
					nextChr = ss.peek();
				}
				if (!isdigit(nextChr) && nextChr != '-' && nextChr != '.') {
					break;
				}
				if (ss >> x) {
					float targetX, targetY;
					if (c == 'H') {
						targetX = x;
						targetY = endY;
					}
					else if (c == 'h') {
						targetX = endX + x;
						targetY = endY;
					}
					m_path->AddLine(endX, endY, targetX, endY);
					endX = targetX;
				}
				else {
					break;
				}
			}
		}
		else if (c == 'V' || c == 'v') {
			float y;
			char nextChr;
			while (true) {
				nextChr = ss.peek();
				while (nextChr == ' ' || nextChr == ',' || nextChr == '\t' || nextChr == '\n') {
					ss.ignore();
					nextChr = ss.peek();
				}
				if (!isdigit(nextChr) && nextChr != '-' && nextChr != '.') {
					break;
				}
				if (ss >> y) {
					float targetX, targetY;
					if (c == 'V') {
						targetX = endX;
						targetY = y;
					}
					else if (c == 'v') {
						targetX = endX;
						targetY = endY + y;
					}
					m_path->AddLine(endX, endY, endX, targetY);
					endY = targetY;
				}
				else {
					break;
				}
			}
		}
		else if (c == 'C' || c == 'c') {
			float x1, y1, x2, y2, x3, y3;
			while (true) {
				char nextChr = ss.peek();
				while (nextChr == ' ' || nextChr == ',' || nextChr == '\t' || nextChr == '\n') {
					ss.ignore();
					nextChr = ss.peek();
				}
				if (!isdigit(nextChr) && nextChr != '-' && nextChr != '.') {
					break;
				}
				if (ss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3) {
					float targetX1, targetY1, targetX2, targetY2, targetX3, targetY3;
					if (c == 'C') {
						targetX1 = x1; targetY1 = y1;
						targetX2= x2; targetY2 = y2;
						targetX3 = x3; targetY3 = y3;
					}
					else if (c == 'c') {
						targetX1 = endX + x1; targetY1 = endY + y1;
						targetX2 = endX + x2; targetY2 = endY + y2;
						targetX3 = endX + x3; targetY3 = endY + y3;
					}
					m_path->AddBezier(endX, endY, targetX1, targetY1, targetX2, targetY2, targetX3, targetY3);
					endX = targetX3;
					endY = targetY3;
				}
				else break;
			}
		}
		else if (c == 'Z' || c == 'z') {
			m_path->CloseFigure();
			endX = startX;
			endY = startY;
		}
		else {
			break;
		}
	}
}

void myPath::draw(Gdiplus::Graphics& g)
{	
	Gdiplus::Matrix originalMatrix;
	g.GetTransform(&originalMatrix);

	Gdiplus::Matrix* transformMatrix = m_transforms.getFinalMatrix();
	if(transformMatrix != nullptr) g.MultiplyTransform(transformMatrix);

	m_fill.setOpacity(m_fill_opacity);
	Gdiplus::SolidBrush fill_pen(m_fill.getColor());
	g.FillPath(&fill_pen, m_path);

	if (m_stroke.getColor().GetAlpha() != 0 && m_stroke_width != 0)
	{
		m_stroke.setOpacity(m_stroke_opacity);
		Gdiplus::Pen stroke_pen(m_stroke.getColor(),m_stroke_width);
		g.DrawPath(&stroke_pen, m_path);
	}

	g.SetTransform(&originalMatrix);
	if (transformMatrix != nullptr) delete transformMatrix;
}