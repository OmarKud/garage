#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>   // GET_X_LPARAM / GET_Y_LPARAM

#include <GL/gl.h>
#include <GL/glu.h>

#include "App.h"
#include <Building.h>

static App* g_app = nullptr;

bool App::Init(HINSTANCE hInst)
{
    g_app = this;
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = App::WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"MyOpenGLApp";
    RegisterClassExW(&wc);

    hwnd = CreateWindowExW(
        0,
        wc.lpszClassName,
        L"Showroom MVP",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100, 1280, 720,
        nullptr, nullptr, hInst, nullptr);

    if (!hwnd) return false;

    hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd{};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 24;

    int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);

    glrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, glrc);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.08f, 0.08f, 0.10f, 1.0f);

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&last);

    // init world with window size
    RECT r; GetClientRect(hwnd, &r);
    int w = r.right - r.left;
    int h = r.bottom - r.top;
    if (h <= 0) h = 1;


    int buildingHeight = 80,
        buildingLength = 350,
        buildingWidth = 500;

    Building b = Building(Point(0, buildingHeight/2, -50), buildingHeight, buildingLength, buildingWidth);

    b.Init();
    world.Init(w, h,b);
    // أخفي الماوس (FPS)
    ShowCursor(FALSE);
    return true;
}

void App::RenderFrame(float dt)
{
    RECT r; GetClientRect(hwnd, &r);
    int w = r.right - r.left;
    int h = r.bottom - r.top;
    if (h <= 0) h = 1;

    world.Resize(w, h);

    glViewport(0, 0, w, h);

    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (double)w / (double)h, 0.05, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    world.Update(dt, input);
    world.Render();

    SwapBuffers(hdc);

    input.EndFrame();
}

int App::Run()
{
    MSG msg{};
    while (true)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) return (int)msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        LARGE_INTEGER now; QueryPerformanceCounter(&now);
        float dt = (float)((double)(now.QuadPart - last.QuadPart) / (double)freq.QuadPart);
        last = now;

        RenderFrame(dt);
    }
}

LRESULT CALLBACK App::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;


    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) { PostQuitMessage(0); return 0; }
        if (g_app) g_app->input.OnKeyDown((unsigned char)wParam);
        return 0;


    case WM_KEYUP:
        if (g_app) g_app->input.OnKeyUp((unsigned char)wParam);
        return 0;


    case WM_MOUSEMOVE:
        if (g_app)
        {
            // mouse pos in client coords
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            // client center
            RECT rc;
            GetClientRect(hWnd, &rc);
            int cx = (rc.right - rc.left) / 2;
            int cy = (rc.bottom - rc.top) / 2;

            // feed input dx/dy relative to center
            g_app->input.OnMouseMove(x, y, cx, cy);

            // recenter cursor (so it doesn't hit screen edges)
            POINT p{ cx, cy };
            ClientToScreen(hWnd, &p);
            SetCursorPos(p.x, p.y);
        }
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
