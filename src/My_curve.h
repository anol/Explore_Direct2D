//
// Created by aeols on 08.11.2022.
//

#ifndef EXPLORE_DIRECT2D_MY_CURVE_H
#define EXPLORE_DIRECT2D_MY_CURVE_H

#include <windowsx.h>
#include <d2d1.h>
#include <cstdint>
#include <iostream>
#include "Explore_utility.h"

struct My_curve {
    const float strokeWidth = 0.5;

    void Draw(ID2D1RenderTarget *pRT, ID2D1SolidColorBrush *pBrush, RECT rect) const {
        pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
        double steepness = 10.0 / (rect.right - rect.left);
        uint16_t midpoint = (rect.left + rect.right) / 2;
        auto min_value = rect.top + 10;
        auto max_value = rect.bottom - 10;
        auto oldX = static_cast<FLOAT>(rect.left);
        auto oldY = static_cast<FLOAT>(min_value);
        for (int x = rect.left; x < rect.right; x++) {
            auto y = Explore_utility::logistic(x, midpoint, steepness, min_value, max_value);
            auto newX = static_cast<FLOAT>(x);
            auto newY = static_cast<FLOAT>(y);
            D2D1_POINT_2F point0 = D2D1::Point2F(oldX, oldY);
            D2D1_POINT_2F point1 = D2D1::Point2F(newX, newY);
            pRT->DrawLine(point0, point1, pBrush, strokeWidth);
            D2D1_POINT_2F point2 = D2D1::Point2F(oldX, static_cast<FLOAT>(rect.bottom) - oldY);
            D2D1_POINT_2F point3 = D2D1::Point2F(newX, static_cast<FLOAT>(rect.bottom) - newY);
            pRT->DrawLine(point2, point3, pBrush, strokeWidth);
            oldX = newX;
            oldY = newY;
        }
    }
};


#endif //EXPLORE_DIRECT2D_MY_CURVE_H
