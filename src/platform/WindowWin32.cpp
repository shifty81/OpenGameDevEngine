/**
 * Win32 Window Implementation
 */

#ifdef _WIN32

#include "ogde/platform/WindowWin32.h"
#include <iostream>

namespace ogde {
namespace platform {

WindowWin32::WindowWin32()
    : m_hwnd(nullptr)
    , m_hinstance(GetModuleHandle(nullptr))
    , m_width(0)
    , m_height(0)
    , m_isActive(false)
    , m_shouldClose(false)
{
}

WindowWin32::~WindowWin32() {
    destroy();
}

bool WindowWin32::create(const std::string& title, uint32_t width, uint32_t height) {
    m_title = title;
    m_width = width;
    m_height = height;

    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hinstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = "OpenGameDevEngineWindow";

    if (!RegisterClassEx(&wc)) {
        std::cerr << "Failed to register window class!" << std::endl;
        return false;
    }

    // Calculate window size with borders
    RECT rect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    // Center window on screen
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    // Create window
    m_hwnd = CreateWindowEx(
        0,
        "OpenGameDevEngineWindow",
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        posX, posY,
        windowWidth, windowHeight,
        nullptr,
        nullptr,
        m_hinstance,
        this  // Pass this pointer to WM_CREATE
    );

    if (!m_hwnd) {
        std::cerr << "Failed to create window!" << std::endl;
        return false;
    }

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
    m_isActive = true;

    return true;
}

void WindowWin32::destroy() {
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }

    UnregisterClass("OpenGameDevEngineWindow", m_hinstance);
    m_isActive = false;
}

bool WindowWin32::processMessages() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            m_shouldClose = true;
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return !m_shouldClose;
}

LRESULT CALLBACK WindowWin32::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowWin32* window = nullptr;

    if (msg == WM_CREATE) {
        // Store the this pointer in the window's user data
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = reinterpret_cast<WindowWin32*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        // Retrieve the this pointer from window's user data
        window = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (window) {
        return window->handleMessage(msg, wParam, lParam);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT WindowWin32::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            m_shouldClose = true;
            if (m_closeCallback) {
                m_closeCallback();
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_SIZE:
        {
            m_width = LOWORD(lParam);
            m_height = HIWORD(lParam);
            if (m_resizeCallback) {
                m_resizeCallback(m_width, m_height);
            }
            return 0;
        }

        case WM_ACTIVATE:
            m_isActive = (LOWORD(wParam) != WA_INACTIVE);
            return 0;
    }

    return DefWindowProc(m_hwnd, msg, wParam, lParam);
}

} // namespace platform
} // namespace ogde

#endif // _WIN32
