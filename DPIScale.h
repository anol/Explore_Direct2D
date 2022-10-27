//
// Created by aeols on 17.10.2022.
//

#ifndef EXPLORE_DIRECT2D_DPISCALE_H
#define EXPLORE_DIRECT2D_DPISCALE_H

#include <Windowsx.h>
#include <d2d1.h>

class DPIScale {
    static float scaleX;
    static float scaleY;

public:
    static void Initialize(ID2D1Factory *pFactory) {
        FLOAT dpiX, dpiY;
        pFactory->GetDesktopDpi(&dpiX, &dpiY);
        scaleX = dpiX / 96.0f;
        scaleY = dpiY / 96.0f;
    }

    template<typename T>
    static float PixelsToDipsX(T x) {
        return static_cast<float>(x) / scaleX;
    }

    template<typename T>
    static float PixelsToDipsY(T y) {
        return static_cast<float>(y) / scaleY;
    }
};

#endif //EXPLORE_DIRECT2D_DPISCALE_H
