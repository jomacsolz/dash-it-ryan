#pragma once
#include <raylib.h>

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, INSTRUCTION1, INSTRUCTION2, ENDING } GameScreen;

const float maxJump = 0.35f;
// const float gravity = 4000.0f;
// const float jumpSpeed = 800.0f;

class Player{
private:
    Texture2D ryan;
    float ground;
    Vector2 velocity;
    Vector2 position;
    float jumpSpeed;
    float gravity;

    // jump variables
    bool isJumping;
    bool onGround;
    float jumpDuration;
    float verticalVelocity;

    // animation variables
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    Rectangle frameRec;

    Sound jumpSFX;
public:
    Player();
    ~Player();
    void Draw(Color tint = WHITE);
    void Update();
    Rectangle GetRect(); // returns player hitbox rectangle
    void DrawHitbox(bool);
    void slowJump(bool);
};