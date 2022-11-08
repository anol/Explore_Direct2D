//
// Created by aeols on 17.10.2022.
//

#ifndef EXPLORE_DIRECT2D_MYELLIPSE_H
#define EXPLORE_DIRECT2D_MYELLIPSE_H

#include <windowsx.h>
#include <d2d1.h>

struct MyEllipse {
    D2D1_ELLIPSE ellipse;
    D2D1_COLOR_F color;

    void Draw(ID2D1RenderTarget *pRT, ID2D1SolidColorBrush *pBrush) {
        pBrush->SetColor(color);
        pRT->FillEllipse(ellipse, pBrush);
        pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
        pRT->DrawEllipse(ellipse, pBrush, 1.0f);
    }

    BOOL HitTest(float x, float y) {
        const float a = ellipse.radiusX;
        const float b = ellipse.radiusY;
        const float x1 = x - ellipse.point.x;
        const float y1 = y - ellipse.point.y;
        const float d = ((x1 * x1) / (a * a)) + ((y1 * y1) / (b * b));
        return d <= 1.0f;
    }
};


#endif //EXPLORE_DIRECT2D_MYELLIPSE_H
