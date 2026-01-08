#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "App.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int)
{
    
    App app;
    if (!app.Init(hInst))
        return 0;
    
    return app.Run();

    
}
