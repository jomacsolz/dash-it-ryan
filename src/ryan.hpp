#pragma once
#include <raylib.h>

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

class Player{
private:
    Texture2D ryan;
    Vector2 position;
    int speed;

    // Animation variables
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    Rectangle frameRec;
public:
    Player();
    ~Player();
    void Draw();
    void Update();
};