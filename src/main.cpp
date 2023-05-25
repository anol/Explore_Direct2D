

#include "MainWindow.h"
#include "Resources.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MainWindow win;
    auto window_name = "Explore Direct2D";
    if (!win.Create(window_name, WS_OVERLAPPEDWINDOW)) {
        return 0;
    }
    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL1));
    ShowWindow(win.Window(), nCmdShow);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(win.Window(), hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 0;
}
