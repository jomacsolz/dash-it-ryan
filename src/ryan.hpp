#pragma once
#include <raylib.h>

class Player{
private:
    Texture2D image;
    Vector2 position;
    int speed;
public:
    Player();
    ~Player();
    void Draw();
    void Update();
};