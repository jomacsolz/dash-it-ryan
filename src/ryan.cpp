#include "ryan.hpp"

Player::Player(){
    ryan = LoadTexture("assets/scarfy.png");
    position = {(1280/25), (3*720)/4};
    speed = 10;

    currentFrame = 0;
    framesCounter = 0;
    framesSpeed = 8;
    frameRec = {0.0f, 0.0f, (float)ryan.width/6, (float)ryan.height};
}

Player::~Player(){
    UnloadTexture(ryan);
}

void Player::Draw(){
    DrawTextureRec(ryan, frameRec, position, WHITE);
}

void Player::Update(){
    framesCounter++;
    if(framesCounter >= (60 / framesSpeed)){
        framesCounter = 0;
        currentFrame++;

        if(currentFrame > 5)
            currentFrame = 0;
        frameRec.x = (float)currentFrame*(float)ryan.width/6;
    }
}