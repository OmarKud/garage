#pragma once
#include <Windows.h>
#include "Input.h"
#include "World.h"

class App {
public:
    bool Init(HINSTANCE hInst);
    int Run();

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void RenderFrame(float dt);

    HWND hwnd = nullptr;
    HDC hdc = nullptr;
    HGLRC glrc = nullptr;

    Input input;
    World world;

    int clientW = 1280;
    int clientH = 720;

    LARGE_INTEGER freq{};
    LARGE_INTEGER last{};
};
