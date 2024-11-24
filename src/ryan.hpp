#pragma once
#include <raylib.h>

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

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