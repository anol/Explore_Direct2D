//
// Created by aeols on 17.10.2022.
//

#include <iostream>
#include "MainWindow.h"
#include "DPIScale.h"
#include "Resources.h"

float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;


D2D1::ColorF::Enum colors[] = {D2D1::ColorF::Yellow, D2D1::ColorF::Salmon, D2D1::ColorF::LimeGreen};

HRESULT MainWindow::CreateDeviceIndependentResources() {
    static const WCHAR msc_fontName[] = L"Verdana";
    static const FLOAT msc_fontSize = 50;
    HRESULT hr;
    if (m_pDWriteFactory == NULL) {
        if (SUCCEEDED(hr)) {
            // Create a DirectWrite factory.
            hr = DWriteCreateFactory(
                    DWRITE_FACTORY_TYPE_SHARED,
                    __uuidof(m_pDWriteFactory),
                    reinterpret_cast<IUnknown **>(&m_pDWriteFactory)
            );
        }
        if (SUCCEEDED(hr)) {
            // Create a DirectWrite text format object.
            hr = m_pDWriteFactory->CreateTextFormat(
                    msc_fontName,
                    NULL,
                    DWRITE_FONT_WEIGHT_NORMAL,
                    DWRITE_FONT_STYLE_NORMAL,
                    DWRITE_FONT_STRETCH_NORMAL,
                    msc_fontSize,
                    L"", //locale
                    &optional_text_format
            );
        }
        if (SUCCEEDED(hr)) {
            // Center the text horizontally and vertically.
            optional_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            optional_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        }
    }
    return hr;
}

HRESULT MainWindow::CreateGraphicsResources() {
    HRESULT hr = S_OK;
    if (pRenderTarget == NULL) {
        RECT rc;
        GetClientRect(m_hwnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
        hr = pFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(m_hwnd, size),
                &pRenderTarget);
        if (SUCCEEDED(hr)) {
            const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
            hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
        }
//        CreateDeviceIndependentResources();
    }
    return hr;
}

void MainWindow::DiscardGraphicsResources() {
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}

void MainWindow::OnPaint() {
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr)) {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);
        pRenderTarget->BeginDraw();
        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
        for (auto i = ellipses.begin(); i != ellipses.end(); ++i) {
            (*i)->Draw(pRenderTarget, pBrush);
        }
        if (Selection()) {
            pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
            pRenderTarget->DrawEllipse(Selection()->ellipse, pBrush, 2.0f);
        }
        the_curve.Draw(pRenderTarget, pBrush, ps.rcPaint);
//        D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
//        pRenderTarget->DrawTextA(L"test", 4, optional_text_format,
//                                 D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height), pBrush);
        hr = pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}

void MainWindow::Resize() {
    if (pRenderTarget != NULL) {
        RECT rc;
        GetClientRect(m_hwnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
        pRenderTarget->Resize(size);
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

void MainWindow::OnLButtonDown(int pixelX, int pixelY, DWORD flags) {
    const float dipX = DPIScale::PixelsToDipsX(pixelX);
    const float dipY = DPIScale::PixelsToDipsY(pixelY);
    if (mode == DrawMode) {
        POINT pt = {pixelX, pixelY};
        if (DragDetect(m_hwnd, pt)) {
            SetCapture(m_hwnd);
            // Start a new ellipse.
            InsertEllipse(dipX, dipY);
        }
    } else {
        ClearSelection();
        if (HitTest(dipX, dipY)) {
            SetCapture(m_hwnd);
            ptMouse = Selection()->ellipse.point;
            ptMouse.x -= dipX;
            ptMouse.y -= dipY;
            SetMode(DragMode);
        }
    }
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::OnLButtonUp() {
    if ((mode == DrawMode) && Selection()) {
        ClearSelection();
        InvalidateRect(m_hwnd, NULL, FALSE);
    } else if (mode == DragMode) {
        SetMode(SelectMode);
    }
    ReleaseCapture();
}

void MainWindow::OnMouseMove(int pixelX, int pixelY, DWORD flags) {
    const float dipX = DPIScale::PixelsToDipsX(pixelX);
    const float dipY = DPIScale::PixelsToDipsY(pixelY);
    if ((flags & MK_LBUTTON) && Selection()) {
        if (mode == DrawMode) {
            // Resize the ellipse.
            const float width = (dipX - ptMouse.x) / 2;
            const float height = (dipY - ptMouse.y) / 2;
            const float x1 = ptMouse.x + width;
            const float y1 = ptMouse.y + height;
            Selection()->ellipse = D2D1::Ellipse(D2D1::Point2F(x1, y1), width, height);
        } else if (mode == DragMode) {
            // Move the ellipse.
            Selection()->ellipse.point.x = dipX + ptMouse.x;
            Selection()->ellipse.point.y = dipY + ptMouse.y;
        }
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

void MainWindow::OnKeyDown(UINT vkey) {
    switch (vkey) {
        case VK_BACK:
        case VK_DELETE:
            if ((mode == SelectMode) && Selection()) {
                ellipses.erase(selection);
                ClearSelection();
                SetMode(SelectMode);
                InvalidateRect(m_hwnd, NULL, FALSE);
            };
            break;
        case VK_LEFT:
            MoveSelection(-1, 0);
            break;
        case VK_RIGHT:
            MoveSelection(1, 0);
            break;
        case VK_UP:
            MoveSelection(0, -1);
            break;
        case VK_DOWN:
            MoveSelection(0, 1);
            break;
        case VK_SPACE:
            if (mode == DrawMode) {
                SetMode(SelectMode);
            } else {
                SetMode(DrawMode);
            }
            break;
    }
}

HRESULT MainWindow::InsertEllipse(float x, float y) {
    try {
        selection = ellipses.insert(
                ellipses.end(),
                std::shared_ptr<MyEllipse>(new MyEllipse()));
        Selection()->ellipse.point = ptMouse = D2D1::Point2F(x, y);
        Selection()->ellipse.radiusX = Selection()->ellipse.radiusY = 2.0f;
        Selection()->color = D2D1::ColorF(colors[nextColor]);
        nextColor = (nextColor + 1) % ARRAYSIZE(colors);
    }
    catch (std::bad_alloc) {
        return E_OUTOFMEMORY;
    }
    return S_OK;
}

BOOL MainWindow::HitTest(float x, float y) {
    for (auto i = ellipses.rbegin(); i != ellipses.rend(); ++i) {
        if ((*i)->HitTest(x, y)) {
            selection = (++i).base();
            return TRUE;
        }
    }
    return FALSE;
}

void MainWindow::MoveSelection(float x, float y) {
    if ((mode == SelectMode) && Selection()) {
        Selection()->ellipse.point.x += x;
        Selection()->ellipse.point.y += y;
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

void MainWindow::SetMode(Mode m) {
    mode = m;
    CHAR *cursor;
    switch (mode) {
        case DrawMode:
            cursor = IDC_CROSS;
            break;
        case SelectMode:
            cursor = IDC_HAND;
            break;
        case DragMode:
            cursor = IDC_SIZEALL;
            break;
    }
    hCursor = LoadCursor(NULL, cursor);
    SetCursor(hCursor);
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            if (FAILED(D2D1CreateFactory(
                    D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) {
                return -1;  // Fail CreateWindowEx.
            }
            DPIScale::Initialize(pFactory);
            SetMode(DrawMode);
            return 0;
        case WM_DESTROY:
            DiscardGraphicsResources();
            SafeRelease(&pFactory);
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            OnPaint();
            return 0;
        case WM_SIZE:
            Resize();
            return 0;
        case WM_LBUTTONDOWN:
            OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD) wParam);
            return 0;
        case WM_LBUTTONUP:
            OnLButtonUp();
            return 0;
        case WM_MOUSEMOVE:
            OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD) wParam);
            return 0;
        case WM_SETCURSOR:
            if (LOWORD(lParam) == HTCLIENT) {
                SetCursor(hCursor);
                return TRUE;
            }
            break;
        case WM_KEYDOWN:
            OnKeyDown((UINT) wParam);
            return 0;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_DRAW_MODE:
                    SetMode(DrawMode);
                    break;
                case ID_SELECT_MODE:
                    SetMode(SelectMode);
                    break;
                case ID_TOGGLE_MODE:
                    if (mode == DrawMode) {
                        SetMode(SelectMode);
                    } else {
                        SetMode(DrawMode);
                    }
                    break;
            }
            return 0;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}