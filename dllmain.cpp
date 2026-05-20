#include <windows.h>

HWND g_hwnd = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CTLCOLORSTATIC:
    {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, RGB(255, 255, 255)); // text color change if u want
        SetBkMode(hdc, TRANSPARENT);
        return (LRESULT)GetStockObject(NULL_BRUSH);
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

DWORD WINAPI MainThread(LPVOID)
{
    Sleep(5000); 

    const wchar_t CLASS_NAME[] = L"MyToast";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    const wchar_t* text = L"Welcome to Project Fracture!"; // change to ur project name

    // measure text
    HDC hdc = GetDC(NULL);
    SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));

    SIZE textSize;
    GetTextExtentPoint32W(hdc, text, lstrlenW(text), &textSize);
    ReleaseDC(NULL, hdc);

    int width = textSize.cx + 80;
    int height = 70;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int x = (screenWidth - width) / 2;
    int y = 40;

    g_hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED,
        CLASS_NAME,
        L"",
        WS_POPUP,
        x, y,
        width, height,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    
    SetLayeredWindowAttributes(g_hwnd, 0, 210, LWA_ALPHA);

    
    HRGN hRgn = CreateRoundRectRgn(0, 0, width, height, 20, 20);
    SetWindowRgn(g_hwnd, hRgn, TRUE);

    
    CreateWindowW(
        L"STATIC",
        text,
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        0, 25,
        width,
        30,
        g_hwnd,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    ShowWindow(g_hwnd, SW_SHOW);
    UpdateWindow(g_hwnd);

    Sleep(4000); 

    DestroyWindow(g_hwnd);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, MainThread, NULL, 0, NULL);
    }
    return TRUE;
}