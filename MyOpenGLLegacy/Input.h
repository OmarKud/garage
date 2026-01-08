#pragma once
#include <cstring> // memcpy

class Input
{
public:
    void OnKeyDown(unsigned char k) { keys[(unsigned char)k] = true; }
    void OnKeyUp(unsigned char k) { keys[(unsigned char)k] = false; }

    void OnMouseMove(int x, int y, int cx, int cy)
    {
        dx += (x - cx);
        dy += (y - cy);
    }

    bool IsDown(unsigned char k) const { return keys[(unsigned char)k]; }

    // true only on first frame the key becomes down
    bool Pressed(unsigned char k) const
    {
        unsigned char kk = (unsigned char)k;
        return keys[kk] && !prevKeys[kk];
    }

    int MouseDX() const { return dx; }
    int MouseDY() const { return dy; }

    void EndFrame()
    {
        std::memcpy(prevKeys, keys, sizeof(keys));
        dx = 0; dy = 0;
    }

private:
    bool keys[256] = { false };
    bool prevKeys[256] = { false };
    int dx = 0, dy = 0;
};
