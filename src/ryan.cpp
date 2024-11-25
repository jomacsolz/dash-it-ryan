#include "ryan.hpp"
#include <raymath.h>

Player::Player(){
    ryan = LoadTexture("assets/ryan.png");
    ground = (3*720)/4;
    velocity = {0.0f, 0.0f};
    position = {(1280/25), ground};
    speed = 20;

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
    if(isOnGround() && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE))){
        velocity.y = -speed;
    }

    position = Vector2Add(position, velocity);

    if(isOnGround()){
        velocity.y = 0;
        position.y = ground;
        framesCounter++;
        if(framesCounter >= (60 / framesSpeed)){
            framesCounter = 0;
            currentFrame++;

            if(currentFrame > 5)
                currentFrame = 0;
            frameRec.x = (float)currentFrame*(float)ryan.width/6;
        }
    }else{
        currentFrame = 3;
        frameRec.x = (float)currentFrame*(float)ryan.width/6;
        velocity.y += 1;
    }
}

bool Player::isOnGround(){
    if(position.y >= ground){
        return true;
    }else
        return false;
}