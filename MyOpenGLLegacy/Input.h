#pragma once

class Input
{
public:
    void OnKeyDown(unsigned char k) { keys[(unsigned char)k] = true; }
    void OnKeyUp(unsigned char k) { keys[(unsigned char)k] = false; }

    // استدعِ هاد من mouse move callback
    void OnMouseMove(int x, int y, int cx, int cy)
    {
        dx += (x - cx);
        dy += (y - cy);
    }

    // استدعِ هاد مرة كل frame بعد ما تستخدم dx/dy
    void EndFrame()
    {
        dx = 0; dy = 0;
    }



    bool IsDown(unsigned char k) const { return keys[(unsigned char)k]; }
    int MouseDX() const { return dx; }
    int MouseDY() const { return dy; }

private:
    bool keys[256] = { false };
    int dx = 0, dy = 0;
};
