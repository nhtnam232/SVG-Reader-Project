#include "RadialGradient.h"

void myRadialGradient::parse(tinyxml2::XMLElement* node) {
    myGradient::parse(node);

    cx = 0.5f; cy = 0.5f; r = 0.5f;
    fx = 0.5f; fy = 0.5f;

    const char* cxStr = node->Attribute("cx");
    if(cxStr) cx = parseSvgValue(cxStr);

    const char* cyStr = node->Attribute("cy");
    if (cyStr) cy = parseSvgValue(cyStr);

    const char* rStr = node->Attribute("r");
    if (rStr) r = parseSvgValue(rStr);

    // fx, fy là tâm tiêu điểm (focus point), nếu không có thì lấy bằng cx, cy
    if (node->Attribute("fx"))
    {
        const char* fxStr = node->Attribute("fx");
        if (fxStr) fx = parseSvgValue(fxStr);
        else fx = cx;
    }
    else fx = cx;

    if (node->Attribute("fy"))
    {
        const char* fyStr = node->Attribute("fy");
        if (fyStr) fy = parseSvgValue(fyStr);
        else fy = cy;
    }
    else fy = cy;
}

Gdiplus::Brush* myRadialGradient::createBrush(const Gdiplus::RectF& objectBounds) {
    // 1. Xử lý đơn vị tọa độ (gradientUnits)
    float realCx, realCy, realR, realFx, realFy;
    if (m_gradientUnits == "userSpaceOnUse") {
        
        // Tọa độ tuyệt đối trong hệ tọa độ của SVG
        realCx = cx;
        realCy = cy;
        realR = r;
        realFx = fx;
        realFy = fy;

        if (m_stops.empty() && !m_parentId.empty()) {
            myGradient* parent = parser.getGradient(m_parentId);
            if (parent) m_stops = parent->getStops();
        }
        if (m_stops.empty()) return nullptr;

        // Đảm bảo có stop tại 0.0 và 1.0
        if (m_stops.front().offset > 0.0f) {
            GradientStop s = m_stops.front(); s.offset = 0.0f;
            m_stops.insert(m_stops.begin(), s);
        }
        if (m_stops.back().offset < 1.0f) {
            GradientStop s = m_stops.back(); s.offset = 1.0f;
            m_stops.push_back(s);
        }
        Gdiplus::GraphicsPath path;
        path.AddEllipse(realCx - realR, realCy - realR, realR * 2, realR * 2);

        

        // Lấy Bounding Box thực tế của Gradient để làm khung cho Bitmap
        // Cần tính cả biến đổi của gradientTransform
        Gdiplus::Matrix* gradM = m_gradientTransform.getFinalMatrix();
        Gdiplus::RectF drawingRect;
        if (gradM) {
            Gdiplus::GraphicsPath transformedPath;
            transformedPath.AddEllipse(realCx - realR, realCy - realR, realR * 2, realR * 2);
            transformedPath.Transform(gradM);
            transformedPath.GetBounds(&drawingRect);
        }
        else {
            path.GetBounds(&drawingRect);
        }

        // Nếu drawingRect quá nhỏ hoặc r = 0, thoát để tránh lỗi chia cho 0
        if (drawingRect.Width <= 0 || drawingRect.Height <= 0) return nullptr;

        Gdiplus::PathGradientBrush* pgb = new Gdiplus::PathGradientBrush(&path);
        pgb->SetCenterPoint(Gdiplus::PointF(realFx, realFy));

        // Thiết lập màu sắc (Đảo ngược vì GDI+ tính 0.0 là biên, 1.0 là tâm)
        int count = (int)m_stops.size();
        std::vector<Gdiplus::Color> colors;
        std::vector<float> positions;
        for (int i = 0; i < count; ++i) {
            Gdiplus::Color base = m_stops[i].m_stop_color.getColor();
            BYTE alpha = (BYTE)(base.GetA() * m_stops[i].m_stop_opacity);
            colors.push_back(Gdiplus::Color(alpha, base.GetR(), base.GetG(), base.GetB()));
            positions.push_back(1.0f - m_stops[i].offset);
        }
        std::reverse(colors.begin(), colors.end());
        std::reverse(positions.begin(), positions.end());
        pgb->SetInterpolationColors(colors.data(), positions.data(), count);

        // Màu biên (Surround Colors) cho toàn bộ các điểm của Path
        Gdiplus::Color edgeCol = colors.front();
        int sCount = path.GetPointCount();
        std::vector<Gdiplus::Color> surroundCols(sCount, edgeCol);
        pgb->SetSurroundColors(surroundCols.data(), &sCount);
        

        // ==== 4. Vẽ vào Bitmap ====
        const int TEX_SIZE = 256;
        Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(TEX_SIZE, TEX_SIZE, PixelFormat32bppARGB);
        Gdiplus::Graphics* gBmp = Gdiplus::Graphics::FromImage(bmp);
        gBmp->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
        gBmp->Clear(edgeCol); // Lấp đầy phần thiếu (Pad)

        // Ma trận đưa tọa độ thế giới (User Space) về khung hình Bitmap (0-256)
        Gdiplus::Matrix bmpMatrix;
        bmpMatrix.Translate(-drawingRect.X, -drawingRect.Y);
        bmpMatrix.Scale(TEX_SIZE / drawingRect.Width, TEX_SIZE / drawingRect.Height, Gdiplus::MatrixOrderAppend);

        if (gradM) {
            Gdiplus::Matrix* combined = gradM;
            combined->Multiply(&bmpMatrix, Gdiplus::MatrixOrderAppend);
            pgb->SetTransform(combined);
        }
        else {
            pgb->SetTransform(&bmpMatrix);
        }

        gBmp->FillRectangle(pgb, 0, 0, TEX_SIZE, TEX_SIZE);

        delete gBmp;
        delete pgb;

        // ==== 5. Tạo TextureBrush và Map ngược lại màn hình ====
        Gdiplus::TextureBrush* finalBrush = new Gdiplus::TextureBrush(bmp, Gdiplus::WrapModeClamp);

        Gdiplus::Matrix finalMatrix;
        // Map từ Bitmap 256x256 ra đúng vùng drawingRect trên màn hình
        finalMatrix.Scale(drawingRect.Width / TEX_SIZE, drawingRect.Height / TEX_SIZE);
        finalMatrix.Translate(drawingRect.X, drawingRect.Y-12, Gdiplus::MatrixOrderAppend);

        finalBrush->SetTransform(&finalMatrix);

        return finalBrush;
        
    }
    else {
        // objectBoundingBox (Mặc định): Tọa độ tỉ lệ (0.0 -> 1.0) so với hình
        realCx = objectBounds.X + cx * objectBounds.Width;
        realCy = objectBounds.Y + cy * objectBounds.Height;
        realR = r * std::max(objectBounds.Width, objectBounds.Height);
        realFx = objectBounds.X + fx * objectBounds.Width;
        realFy = objectBounds.Y + fy * objectBounds.Height;
    }

    // 2. Tạo một đường Path hình tròn để làm biên cho Gradient
    Gdiplus::GraphicsPath path;
    path.AddEllipse(cx - r, cy - r, r * 2, r * 2);

    Gdiplus::Matrix* gradMatrix = m_gradientTransform.getFinalMatrix();
    if (gradMatrix) {
        // --- BƯỚC QUAN TRỌNG ---
        Gdiplus::Matrix invMatrix;
        invMatrix.Multiply(gradMatrix);
        invMatrix.Invert(); // Nghịch đảo ma trận

        // Biến dạng cái khuôn ngược lại trước
        path.Transform(&invMatrix);
    }

    // 3. Khởi tạo PathGradientBrush từ path
    Gdiplus::PathGradientBrush* pgb = new Gdiplus::PathGradientBrush(&path);
    pgb->SetWrapMode(Gdiplus::WrapModeClamp);

    // href
    if (m_stops.empty() && !m_parentId.empty()) {
        myGradient* parent = parser.getGradient(m_parentId);
        if (parent) {
            m_stops = (parent->getStops());
        }
    }

    // 4. Thiết lập màu tại tâm và màu tại biên (Dựa trên m_stops)
    if (!m_stops.empty()) {
        // 1. Kiểm tra nếu stop đầu tiên không phải 0.0
        if (m_stops.front().offset > 0.0f) {
            GradientStop first = m_stops.front();
            first.offset = 0.0f;
            m_stops.insert(m_stops.begin(), first);
        }

        // 2. Kiểm tra nếu stop cuối cùng không phải 1.0
        if (m_stops.back().offset < 1.0f) {
            GradientStop last = m_stops.back();
            last.offset = 1.0f;
            m_stops.push_back(last);
        }
        // GDI+ PathGradient dùng màu tâm là stop đầu tiên, màu biên là stop cuối cùng
        pgb->SetCenterColor(m_stops.front().m_stop_color.getColor());

        int count = m_stops.size();
        if (count == 1) {
            Gdiplus::Color col = m_stops[0].m_stop_color.getColor();
            pgb->SetCenterColor(col);
            int surroundCount = 1;
            pgb->SetSurroundColors(&col, &surroundCount);
        }
        else {
            std::vector<Gdiplus::Color> colors;
            std::vector<float> positions;

            for (int i = 0; i < count; ++i) {
                Gdiplus::Color baseColor = m_stops[i].m_stop_color.getColor();

                // Tính toán Alpha: Lấy Alpha gốc nhân với stop-opacity (0.0 - 1.0)
                BYTE finalAlpha = static_cast<BYTE>(baseColor.GetA() * m_stops[i].m_stop_opacity);

                // Tạo màu mới với Alpha đã được điều chỉnh
                Gdiplus::Color stopColor(finalAlpha, baseColor.GetR(), baseColor.GetG(), baseColor.GetB());

                colors.push_back(stopColor);

                // Đảo ngược vị trí: 0% SVG -> 1.0 GDI (Tâm), 100% SVG -> 0.0 GDI (Biên)
                positions.push_back(1.0f - m_stops[i].offset);
            }

            // GDI+ yêu cầu mảng positions phải tăng dần từ 0.0 đến 1.0
            // Vì ta dùng (1.0 - offset), mảng hiện tại đang giảm dần -> Cần lật ngược cả 2 mảng


            std::reverse(colors.begin(), colors.end());
            std::reverse(positions.begin(), positions.end());

            // Đảo ngược nếu cần vì GDI+ tính từ biên vào tâm
            pgb->SetInterpolationColors(colors.data(), positions.data(), count);

            Gdiplus::Color finalEdgeColor = m_stops.back().m_stop_color.getColor();

            int sCount = 1;
            pgb->SetSurroundColors(&finalEdgeColor, &sCount);
        }
    }

    // Gradient Transform
    Gdiplus::Matrix totalMatrix;
    totalMatrix.Scale(objectBounds.Width, objectBounds.Height);
    totalMatrix.Translate(objectBounds.X, objectBounds.Y, Gdiplus::MatrixOrderAppend);
    if (gradMatrix) {
        // SVG quy định: gradientTransform được áp dụng TRƯỚC khi map vào vật thể
        // (Trong không gian đơn vị)
        Gdiplus::Matrix combined;
        combined.Multiply(gradMatrix);
        combined.Multiply(&totalMatrix, Gdiplus::MatrixOrderAppend);

        pgb->SetTransform(&combined);
        delete gradMatrix;
    }
    else {
        // Nếu không có gradientTransform, chỉ cần map từ 0->1 sang Pixel
        pgb->SetTransform(&totalMatrix);
    }

    // 5. Thiết lập Center Point (Focal Point) trong hệ đơn vị
    // GDI+ sẽ tự động nhân điểm này với totalMatrix để ra vị trí thực
    pgb->SetCenterPoint(Gdiplus::PointF(fx, fy));

    const int TEX_SIZE = 256;
    Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(TEX_SIZE, TEX_SIZE, PixelFormat32bppARGB);
    Gdiplus::Graphics* gBmp = Gdiplus::Graphics::FromImage(bmp);

    // Đảm bảo vẽ mượt
    gBmp->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    // 1. Điền phần thiếu bằng màu Stop cuối cùng (Quy tắc Pad)
    Gdiplus::Color edgeColor = m_stops.back().m_stop_color.getColor();
    // Áp dụng opacity cho màu biên nếu có
    Gdiplus::Color finalEdgeColor(
        (BYTE)(edgeColor.GetA() * m_stops.back().m_stop_opacity),
        edgeColor.GetR(), edgeColor.GetG(), edgeColor.GetB()
    );
    gBmp->Clear(finalEdgeColor);

    // 2. Thiết lập Transform cho pgb để vẽ TRONG bitmap (không gian 0-1 -> 0-256)
    Gdiplus::Matrix bmpMatrix;
    bmpMatrix.Scale(TEX_SIZE, TEX_SIZE); // Phóng từ 0-1 lên 0-256

    // Nếu có gradientTransform, phải áp dụng nó TRƯỚC khi scale lên bitmap
    Gdiplus::Matrix* gradM = m_gradientTransform.getFinalMatrix();
    if (gradM) {
        Gdiplus::Matrix combined;
        combined.Multiply(gradM);
        combined.Multiply(&bmpMatrix, Gdiplus::MatrixOrderAppend);
        pgb->SetTransform(&combined);
        delete gradM;
    }
    else {
        pgb->SetTransform(&bmpMatrix);
    }

    // 3. Vẽ PGB lên toàn bộ bitmap
    // Lưu ý: PGB chỉ vẽ bên trong cái 'path' (hình tròn). 
    // Những gì ngoài path sẽ giữ nguyên màu finalEdgeColor của lệnh Clear phía trên.
    gBmp->FillRectangle(pgb, 0, 0, TEX_SIZE, TEX_SIZE);

    // Dọn dẹp PGB vì ta đã có dữ liệu trong Bitmap
    delete gBmp;
    delete pgb;

    // 4. Tạo TextureBrush từ Bitmap đã xử lý
    Gdiplus::TextureBrush* finalBrush = new Gdiplus::TextureBrush(bmp, Gdiplus::WrapModeClamp);

    // 5. Map TextureBrush từ không gian Bitmap (0-256) ra objectBounds thực tế
    Gdiplus::Matrix finalMatrix;
    finalMatrix.Scale(objectBounds.Width / TEX_SIZE, objectBounds.Height / TEX_SIZE);
    finalMatrix.Translate(objectBounds.X, objectBounds.Y, Gdiplus::MatrixOrderAppend);

    finalBrush->SetTransform(&finalMatrix);

    return finalBrush;
}

/*
Gdiplus::Color interpolateColor(const std::vector<GradientStop>& stops, float t) {
    for (size_t i = 1; i < stops.size(); ++i) {
        if (t <= stops[i].offset) {
            float t0 = stops[i - 1].offset;
            float t1 = stops[i].offset;
            float local = (t - t0) / (t1 - t0);

            auto c0 = stops[i - 1].m_stop_color.getColor();
            auto c1 = stops[i].m_stop_color.getColor();

            float a0 = c0.GetA() * stops[i - 1].m_stop_opacity;
            float a1 = c1.GetA() * stops[i].m_stop_opacity;

            BYTE a = BYTE(a0 + local * (a1 - a0));
            BYTE r = BYTE(c0.GetR() + local * (c1.GetR() - c0.GetR()));
            BYTE g = BYTE(c0.GetG() + local * (c1.GetG() - c0.GetG()));
            BYTE b = BYTE(c0.GetB() + local * (c1.GetB() - c0.GetB()));

            return Gdiplus::Color(a, r, g, b);
        }
    }
    return stops.back().m_stop_color.getColor();
}

Gdiplus::Brush* myRadialGradient::createBrush(const Gdiplus::RectF& objectBounds)
{
    // ==== 1. Chuẩn hóa stops ====
    if (m_stops.empty() && !m_parentId.empty()) {
        myGradient* parent = parser.getGradient(m_parentId);
        if (parent)
            m_stops = parent->getStops();
    }

    if (m_stops.empty())
        return nullptr;

    if (m_stops.front().offset > 0.0f) {
        GradientStop s = m_stops.front();
        s.offset = 0.0f;
        m_stops.insert(m_stops.begin(), s);
    }
    if (m_stops.back().offset < 1.0f) {
        GradientStop s = m_stops.back();
        s.offset = 1.0f;
        m_stops.push_back(s);
    }

    Gdiplus::Matrix invM;
    Gdiplus::Matrix* gradM = m_gradientTransform.getFinalMatrix();
    if (gradM) {
        invM.Multiply(gradM);
        invM.Invert(); // Nghịch đảo: nếu gradM là scale 0.8, invM sẽ là scale 1.25
        delete gradM;
    }

    // ==== 2. Kích thước texture ====
    const int TEX_SIZE = 256;   // đủ mịn
    const float texCx = cx * TEX_SIZE;
    const float texCy = cy * TEX_SIZE;
    const float texR = r * TEX_SIZE;
    const float texFx = fx * TEX_SIZE; // Lấy thêm fx, fy
    const float texFy = fy * TEX_SIZE;

    // ==== 3. Tạo bitmap radial gradient ====
    Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(TEX_SIZE, TEX_SIZE, PixelFormat32bppARGB);

    Gdiplus::BitmapData data;
    Gdiplus::Rect rect(0, 0, TEX_SIZE, TEX_SIZE);
    bmp->LockBits(&rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &data);

    BYTE* pixels = (BYTE*)data.Scan0;

    for (int y = 0; y < TEX_SIZE; ++y) {
        for (int x = 0; x < TEX_SIZE; ++x) {
            Gdiplus::PointF p((float)x, (float)y);

            // BIẾN ĐỔI NGƯỢC: Đưa pixel hiện tại về không gian "tròn" ban đầu
            //invM.TransformPoints(&p);

            float dPx = p.X - texFx;
            float dPy = p.Y - texFy;

            float dCx = texCx - texFx;
            float dCy = texCy - texFy;

            // Khoảng cách từ F → P
            float distFP = std::sqrt(dPx * dPx + dPy * dPy);

            // Khoảng cách từ F → C
            float distFC = std::sqrt(dCx * dCx + dCy * dCy);

            // SVG approximation: bán kính hiệu dụng
            float effectiveR = distFC + texR;

            // Chuẩn hóa
            float t = distFP / effectiveR;
            t = std::max(0.0f, std::min(t, 1.0f));

            Gdiplus::Color c = interpolateColor(m_stops, t);

            int idx = y * data.Stride + x * 4;
            pixels[idx + 0] = c.GetB();
            pixels[idx + 1] = c.GetG();
            pixels[idx + 2] = c.GetR();
            pixels[idx + 3] = c.GetA();
        }
    }

    bmp->UnlockBits(&data);

    // ==== 4. Tạo TextureBrush ====
    Gdiplus::TextureBrush* brush = new Gdiplus::TextureBrush(bmp, Gdiplus::WrapModeClamp);

    // ==== 5. Map bitmap (0–1) → objectBounds ====
    Gdiplus::Matrix brushMatrix;

    brushMatrix.Scale(
        objectBounds.Width / TEX_SIZE,
        objectBounds.Height / TEX_SIZE
    );

    brushMatrix.Translate(
        objectBounds.X,
        objectBounds.Y,
        Gdiplus::MatrixOrderAppend
    );

    // ==== 6. Áp gradientTransform (SVG: TRƯỚC mapping) ====
    brush->SetTransform(&brushMatrix);
    return brush;
}
*/