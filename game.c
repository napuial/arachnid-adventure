#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "Game";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Arachnid Adventure",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HBITMAP hBitmap = NULL;

    switch (uMsg) {
        case WM_CREATE: {
            hBitmap = (HBITMAP)LoadImage(
                NULL,
                "player.bmp",
                IMAGE_BITMAP,
                0, 0,
                LR_LOADFROMFILE
            );

            if (!hBitmap) {
                MessageBox(hwnd, "Nie udało się wczytać obrazu!", "Błąd", MB_OK | MB_ICONERROR);
            }
            break;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (hBitmap) {
                HDC hdcMem = CreateCompatibleDC(hdc);
                HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

                BITMAP bitmap;
                GetObject(hBitmap, sizeof(BITMAP), &bitmap);

                BitBlt(hdc, 100, 100, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

                SelectObject(hdcMem, hOldBitmap);
                DeleteDC(hdcMem);
            } else {
                TextOut(hdc, 100, 100, "Brak obrazu!", 12);
            }

            EndPaint(hwnd, &ps);
            break;
        }

        case WM_DESTROY: {
            if (hBitmap) {
                DeleteObject(hBitmap);
            }
            PostQuitMessage(0);
            break;
        }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
