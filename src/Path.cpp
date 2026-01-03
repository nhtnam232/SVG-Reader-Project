#include "Path.h"
#include "Parser.h"

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
	float lastcontrolX = 0.0f, lastcontrolY = 0.0f;
	char lastCommand = ' ';
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
						lastCommand = 'L';
					}
					else {
						m_path->StartFigure();
						startX = targetX;
						startY = targetY;
						lastCommand = 'M';
					}
					endX = targetX;
					endY = targetY;
					cnt += 1;
					lastcontrolX = endX;
					lastcontrolY = endY;

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
					lastCommand = c;
					lastcontrolX = endX;
					lastcontrolY = endY;
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
					lastCommand = c;
					lastcontrolX = endX;
					lastcontrolY = endY;
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
					lastCommand = c;
					lastcontrolX = endX;
					lastcontrolY = endY;
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
						targetX2 = x2; targetY2 = y2;
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
					lastcontrolX = targetX2;
					lastcontrolY = targetY2;
					lastCommand = c;
				}
				else break;
			}
		}
		else if (c == 'S' || c == 's') {
			float x2, y2, x, y;
			while (true) {
				char nextChr = ss.peek();
				while (nextChr == ' ' || nextChr == ',' || nextChr == '\t' || nextChr == '\n') {
					ss.ignore();
					nextChr = ss.peek();
				}
				if (!isdigit(nextChr) && nextChr != '-' && nextChr != '.') {
					break;
				}
				if (ss >> x2 >> y2 >> x >> y) {
					float targetX2, targetY2, targetX, targetY;
					if (c == 'S') {
						targetX2 = x2; targetY2 = y2;
						targetX = x; targetY = y;
					}
					else if (c == 's') {
						targetX2 = endX + x2; targetY2 = endY + y2;
						targetX = endX + x; targetY = endY + y;
					}
					float ctrl1X, ctrl1Y;
					if (lastCommand == 'C' || lastCommand == 'c' || lastCommand == 'S' || lastCommand == 's') {
						ctrl1X = 2 * endX - lastcontrolX;
						ctrl1Y = 2 * endY - lastcontrolY;
					}
					else {
						ctrl1X = endX;
						ctrl1Y = endY;
					}
					m_path->AddBezier(endX, endY, ctrl1X, ctrl1Y, targetX2, targetY2, targetX, targetY);
					lastcontrolX = targetX2;
					lastcontrolY = targetY2;
					endX = targetX;
					endY = targetY;
					lastCommand = c;

				}
			}
		}
		else if (c == 'Q' || c == 'q') {
			float x1, y1, x, y;
			while (true) {
				char nextChr = ss.peek();
				while (nextChr == ' ' || nextChr == ',' || nextChr == '\t' || nextChr == '\n') {
					ss.ignore();
					nextChr = ss.peek();
				}
				if (!isdigit(nextChr) && nextChr != '-' && nextChr != '.') break;
				if (ss >> x1 >> y1 >> x >> y) {
					float targetX1, targetY1, targetX, targetY;
					if (c == 'Q') {
						targetX1 = x1; targetY1 = y1;
						targetX = x; targetY = y;
					}
					else {
						targetX1 = endX + x1; targetY1 = endY + y1;
						targetX = endX + x; targetY = endY + y;
					}
					float c1x = endX + (2.0f / 3.0f) * (targetX1 - endX);
					float c1y = endY + (2.0f / 3.0f) * (targetY1 - endY);

					float c2x = targetX + (2.0f / 3.0f) * (targetX1 - targetX);
					float c2y = targetY + (2.0f / 3.0f) * (targetY1 - targetY);

					m_path->AddBezier(endX, endY, c1x, c1y, c2x, c2y, targetX, targetY);


					endX = targetX;
					endY = targetY;
					lastcontrolX = targetX1;
					lastcontrolY = targetY1;
					lastCommand = c;
				}
				else {
					break;
				}
			}
		}
		else if (c == 'T' || c == 't') {
			float x, y;
			while (true) {
				char nextChr = ss.peek();
				while (nextChr == ' ' || nextChr == ',' || nextChr == '\t' || nextChr == '\n') {
					ss.ignore();
					nextChr = ss.peek();
				}
				if (!isdigit(nextChr) && nextChr != '-' && nextChr != '.') break;

				if (ss >> x >> y) {
					float targetX, targetY;
					if (c == 'T') {
						targetX = x; targetY = y;
					}
					else { 
						targetX = endX + x; targetY = endY + y;
					}

					float ctrlX, ctrlY;

					if (lastCommand == 'Q' || lastCommand == 'q' || lastCommand == 'T' || lastCommand == 't') {
						ctrlX = 2 * endX - lastcontrolX;
						ctrlY = 2 * endY - lastcontrolY;
					}
					else {
						ctrlX = endX;
						ctrlY = endY;
					}

					float c1x = endX + (2.0f / 3.0f) * (ctrlX - endX);
					float c1y = endY + (2.0f / 3.0f) * (ctrlY - endY);

					float c2x = targetX + (2.0f / 3.0f) * (ctrlX - targetX);
					float c2y = targetY + (2.0f / 3.0f) * (ctrlY - targetY);

					m_path->AddBezier(endX, endY, c1x, c1y, c2x, c2y, targetX, targetY);

					endX = targetX;
					endY = targetY;
					lastcontrolX = ctrlX;
					lastcontrolY = ctrlY;
					lastCommand = c;
				}
				else break;
			}
		}
		else if (c == 'A' || c == 'a') {
			double rx, ry, x_axis_rotation, x_val, y_val;
			bool large_arc_flag, sweep_flag;
			const double Pi = 3.14159265358979323846;

			while (true) {
				char nextChr = ss.peek();
				while (nextChr == ' ' || nextChr == ',' || nextChr == '\t' || nextChr == '\n') {
					ss.ignore();
					nextChr = ss.peek();
				}
				if (!isdigit(nextChr) && nextChr != '-' && nextChr != '.') break;

				if (!(ss >> rx >> ry >> x_axis_rotation)) break;

				char flagChar;
				if (!(ss >> flagChar)) break;
				large_arc_flag = (flagChar == '1');


				if (!(ss >> flagChar)) break;
				sweep_flag = (flagChar == '1');


				if (!(ss >> x_val >> y_val)) break;


				double sx = endX;
				double sy = endY;
				double ex = (c == 'A') ? x_val : endX + x_val;
				double ey = (c == 'A') ? y_val : endY + y_val;

				if (sx == ex && sy == ey) {}
				else if (rx == 0.0 || ry == 0.0) {
					m_path->AddLine((float)sx, (float)sy, (float)ex, (float)ey);
				}
				else {
					double angle = x_axis_rotation * Pi / 180.0;
					double cosAngle = cos(angle);
					double sinAngle = sin(angle);

					double dx2 = (sx - ex) / 2.0;
					double dy2 = (sy - ey) / 2.0;
					double x1 = cosAngle * dx2 + sinAngle * dy2;
					double y1 = -sinAngle * dx2 + cosAngle * dy2;

					rx = fabs(rx);
					ry = fabs(ry);
					double Prx = rx * rx;
					double Pry = ry * ry;
					double Px1 = x1 * x1;
					double Py1 = y1 * y1;

					double lambda = Px1 / Prx + Py1 / Pry;
					if (lambda > 1.0) {
						double scale = sqrt(lambda);
						rx *= scale; ry *= scale;
						Prx = rx * rx; Pry = ry * ry;
					}

					double sign = (large_arc_flag == sweep_flag ? -1.0 : 1.0);

					double num = Prx * Pry - Prx * Py1 - Pry * Px1;
					double den = Prx * Py1 + Pry * Px1;
					if (den < 1e-6) den = 1e-6;
					if (num < 0.0) num = 0.0;

					double coef = sign * sqrt(num / den);
					double cx1 = coef * ((rx * y1) / ry);
					double cy1 = coef * -((ry * x1) / rx);

					double cx = cosAngle * cx1 - sinAngle * cy1 + (sx + ex) / 2.0;
					double cy = sinAngle * cx1 + cosAngle * cy1 + (sy + ey) / 2.0;

					double ux = (x1 - cx1) / rx;
					double uy = (y1 - cy1) / ry;
					double vx = (-x1 - cx1) / rx;
					double vy = (-y1 - cy1) / ry;

					double startAngle = atan2(uy, ux);
					double endAngle = atan2(vy, vx);
					double dAngle = endAngle - startAngle;

					if (dAngle > Pi) dAngle -= 2.0 * Pi;
					if (dAngle < -Pi) dAngle += 2.0 * Pi;

					if (!sweep_flag && dAngle > 0.0) dAngle -= 2.0 * Pi;
					else if (sweep_flag && dAngle < 0.0) dAngle += 2.0 * Pi;

					int segments = (int)ceil(std::abs(dAngle) / (Pi / 2.0));
					if (segments < 1) segments = 1;
					double delta = dAngle / segments;
					double t = (4.0 / 3.0) * tan(delta / 4.0);

					double currentAngle = startAngle;
					auto mapPt = [&](double ang) -> Gdiplus::PointF {
						double u = cos(ang); double v = sin(ang);
						double tmpx = rx * u; double tmpy = ry * v;
						return Gdiplus::PointF((float)(cosAngle * tmpx - sinAngle * tmpy + cx),
							(float)(sinAngle * tmpx + cosAngle * tmpy + cy));
						};

					for (int i = 0; i < segments; i++) {
						double theta1 = currentAngle;
						double theta2 = currentAngle + delta;
						double cos1 = cos(theta1), sin1 = sin(theta1);
						double cos2 = cos(theta2), sin2 = sin(theta2);

						double e1x = cos1 - t * sin1; double e1y = sin1 + t * cos1;
						double e2x = cos2 + t * sin2; double e2y = sin2 - t * cos2;

						auto transCP = [&](double u, double v) -> Gdiplus::PointF {
							double tmpx = rx * u; double tmpy = ry * v;
							return Gdiplus::PointF((float)(cosAngle * tmpx - sinAngle * tmpy + cx),
								(float)(sinAngle * tmpx + cosAngle * tmpy + cy));
							};

						Gdiplus::PointF cp1 = transCP(e1x, e1y);
						Gdiplus::PointF cp2 = transCP(e2x, e2y);
						Gdiplus::PointF dest = mapPt(theta2);

						if (i == segments - 1) { dest.X = (float)ex; dest.Y = (float)ey; }
						m_path->AddBezier((float)sx, (float)sy, cp1.X, cp1.Y, cp2.X, cp2.Y, dest.X, dest.Y);

						sx = dest.X; sy = dest.Y;
						currentAngle += delta;
					}
				}
				endX = ex;
				endY = ey;
				lastcontrolX = endX;
				lastcontrolY = endY;
				lastCommand = c;
			}
		}
		else if (c == 'Z' || c == 'z') {
			m_path->CloseFigure();
			endX = startX;
			endY = startY;
			lastCommand = c;
			lastcontrolX = endX;
			lastcontrolY = endY;
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
	if (transformMatrix != nullptr)
		g.MultiplyTransform(transformMatrix);

	m_path->SetFillMode(Gdiplus::FillModeWinding);
	Gdiplus::RectF bounds;
	m_path->GetBounds(&bounds);

	Gdiplus::Brush* fillBrush = nullptr;
	if (!m_fill_gradient_id.empty()) {
		myGradient* grad = parser.getGradient(m_fill_gradient_id);
		if (grad) fillBrush = grad->createBrush(bounds);
	}
	if (!fillBrush) {
		Color fill = m_fill;
		fill.setOpacity(m_fill_opacity);
		fillBrush = new Gdiplus::SolidBrush(fill.getColor());
	}
	g.FillPath(fillBrush, m_path);
	delete fillBrush;

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
		g.DrawPath(&pen, m_path);
		delete strokeBrush;
	}

	g.SetTransform(&originalMatrix);
	if (transformMatrix != nullptr) delete transformMatrix;
}