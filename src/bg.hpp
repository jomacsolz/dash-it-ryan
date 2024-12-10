#pragma once
#include <iostream>
#include <raylib.h>

class BG{
private:
    Texture2D background;
    Texture2D midground;
    Texture2D midground2;
    Texture2D foreground;
    float scrollBack;
    float scrollMid;
    float scrollMid2;
    float scrollFore;
    float bgSpeed;
public:
    BG();
    ~BG();
    void Draw();
    void Update();
    void SpeedUp(float);
    void SpeedDown();
    void resetSpeed();
    float scaleImg(Texture2D img);
    float getSpeed();
    void setSpeed(float);
};

