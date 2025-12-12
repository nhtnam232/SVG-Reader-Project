#include "Path.h"

void myPath::parse(tinyxml2::XMLElement* node) {
	myShape::parse(node);
	const char* d = node->Attribute("d");
	if (d == nullptr) return;
	string pathStr = d;
	for (int i = 0; i < pathStr.size(); i++) {
		if (pathStr[i] == ',') pathStr[i] = ' ';
	}
	stringstream ss(pathStr);
	char c;
	float x, y, x1, y1, x2, y2, startX = 0.0f, startY = 0.0f, endX = 0.0f, endY = 0.0f;
	while (ss >> c) {
		switch (c) {
		case 'M':
			ss >> x >> y;
			startX = x;
			startY = y;
			endX = x;
			endY = y;
			m_path->StartFigure();
			break;
		case 'L':
			ss >> x >> y;
			m_path->AddLine(endX, endY, x, y);
			endX = x;
			endY = y;
			break;
		case 'H':
			ss >> x;
			m_path->AddLine(endX, endY, x, endY);
			endX = x;
			break;
		case 'V':
			ss >> y;
			m_path->AddLine(endX, endY, endX, y);
			endY = y;
			break;
		case 'C':
			ss >> x1 >> y1 >> x2 >> y2 >> x >> y;
			m_path->AddBezier(endX, endY, x1, y1, x2, y2, x, y);
			endX = x;
			endY = y;
			break;
		case 'Z':
			m_path->CloseFigure();
			endX = startX;
			endY = startY;
			break;
		default:
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