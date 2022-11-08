//
// Created by aeols on 17.10.2022.
//

#ifndef EXPLORE_DIRECT2D_MAINWINDOW_H
#define EXPLORE_DIRECT2D_MAINWINDOW_H


#include "BaseWindow.h"
#include "MyEllipse.h"

#include <windowsx.h>
#include <d2d1.h>
#include <list>
#include <memory>

class MainWindow : public BaseWindow<MainWindow> {
    enum Mode {
        DrawMode,
        SelectMode,
        DragMode
    };
    HCURSOR hCursor;
    ID2D1Factory *pFactory;
    ID2D1HwndRenderTarget *pRenderTarget;
    ID2D1SolidColorBrush *pBrush;
    D2D1_POINT_2F ptMouse;
    Mode mode;
    size_t nextColor;
    std::list<std::shared_ptr<MyEllipse>> ellipses;
    std::list<std::shared_ptr<MyEllipse>>::iterator selection;

    std::shared_ptr<MyEllipse> Selection() {
        if (selection == ellipses.end()) {
            return nullptr;
        } else {
            return (*selection);
        }
    }

    template<class T>
    void SafeRelease(T **ppT) {
        if (*ppT) {
            (*ppT)->Release();
            *ppT = NULL;
        }
    }

private:
    void ClearSelection() { selection = ellipses.end(); }

    HRESULT InsertEllipse(float x, float y);

    BOOL HitTest(float x, float y);

    void SetMode(Mode m);

    void MoveSelection(float x, float y);

    HRESULT CreateGraphicsResources();

    void DiscardGraphicsResources();

    void OnPaint();

    void Resize();

    void OnLButtonDown(int pixelX, int pixelY, DWORD flags);

    void OnLButtonUp();

    void OnMouseMove(int pixelX, int pixelY, DWORD flags);

    void OnKeyDown(UINT vkey);

public:
    MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL),
                   ptMouse(D2D1::Point2F()), nextColor(0), selection(ellipses.end()) {
    }

    PCWSTR ClassName() const { return L"Circle Window Class"; }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif //EXPLORE_DIRECT2D_MAINWINDOW_H
