#pragma once
#include <raylib.h>

class Obstacle{
private:
    Texture2D obs;
    Vector2 position;
    Rectangle hitbox;
    float speed;
public:
    Obstacle();
    ~Obstacle();
    void InitObstacle();
    void Draw();
    void Update();
    Rectangle GetRect();
};