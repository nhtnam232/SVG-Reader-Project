#include "Line.h"
#include "Parser.h"

void myLine::parse(tinyxml2::XMLElement* node)
{
	myShape::parse(node);
	node->QueryFloatAttribute("x1", &m_x1);
	node->QueryFloatAttribute("x2", &m_x2);
	node->QueryFloatAttribute("y1", &m_y1);
	node->QueryFloatAttribute("y2", &m_y2);
}

void myLine::draw(Gdiplus::Graphics& g)
{
	Gdiplus::Matrix originalMatrix;
	g.GetTransform(&originalMatrix);

	Gdiplus::Matrix* transformMatrix = m_transforms.getFinalMatrix();
	if (transformMatrix != nullptr)
		g.MultiplyTransform(transformMatrix);

	// bounds
	Gdiplus::PointF p1(m_x1, m_y1);
	Gdiplus::PointF p2(m_x2, m_y2);
	Gdiplus::GraphicsPath path;
	path.AddLine(p1, p2);
	Gdiplus::Pen pen(m_stroke.getColor(), m_stroke_width);
	Gdiplus::GraphicsPath* p = static_cast<Gdiplus::GraphicsPath*>(path.Clone());
		// NỞ path theo stroke
	p->Widen(&pen);
	Gdiplus::RectF bounds;
	p->GetBounds(&bounds);
	delete p;

	if (m_stroke_width != 0 && (!m_stroke_gradient_id.empty() || m_stroke.getColor().GetAlpha() != 0))
	{
		Gdiplus::Brush* strokeBrush = nullptr;
		if (!m_stroke_gradient_id.empty()) {
			myGradient* grad = parser.getGradient(m_stroke_gradient_id);
			if (grad) strokeBrush = grad->createBrush(bounds);
		}

		if (!strokeBrush) {
			Color stroke = m_stroke;
			stroke.setOpacity(m_stroke_opacity);
			strokeBrush = new Gdiplus::SolidBrush(stroke.getColor());
		}

		Gdiplus::Pen pen(strokeBrush, m_stroke_width);
		g.DrawLine(&pen, m_x1, m_y1, m_x2, m_y2);
		delete strokeBrush;
	}


	g.SetTransform(&originalMatrix);
	if (transformMatrix != nullptr) delete transformMatrix;
}