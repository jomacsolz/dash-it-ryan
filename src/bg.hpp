#pragma once
#include <iostream>
#include <raylib.h>

class BG{
private:
    Texture2D background;
    Texture2D midground;
    Texture2D foreground;
    float scrollBack;
    float scrollMid;
    float scrollFore;
    float bgSpeed;
public:
    BG();
    ~BG();
    void Draw();
    void Update();
    void SpeedUp(float);
    void resetSpeed();
};