#include "ryan.hpp"
#include <raymath.h>

Player::Player(){
    ryan = LoadTexture("assets/ryan.png");
    ground = (3*720)/4;
    velocity = {0.0f, 0.0f};
    position = {(1280/25), ground};
    jumpSFX = LoadSound("assets/audio/jump.wav");

    // jump vars
    isJumping = false;
    onGround = true;
    jumpDuration = 0.0f;
    verticalVelocity = 0.0f;

    // Animation variables
    currentFrame = 0;
    framesCounter = 0;
    framesSpeed = 8;
    frameRec = {0.0f, 0.0f, (float)ryan.width/6, (float)ryan.height};
}

Player::~Player(){
    UnloadTexture(ryan);
    UnloadSound(jumpSFX);
}

void Player::Draw(){
    DrawTextureRec(ryan, frameRec, position, WHITE);
}

void Player::Update(){
    if(onGround && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE))){
        isJumping = true;
        jumpDuration = 0.0f;
        onGround = false;
        PlaySound(jumpSFX);
    }

    if(isJumping){
        if((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && jumpDuration < maxJump)
            jumpDuration += GetFrameTime();
        else
            isJumping = false;
    }

    if(isJumping){
        float t = jumpDuration/maxJump;
        verticalVelocity = -jumpSpeed * (1-t*t);
    }else
        verticalVelocity += gravity * GetFrameTime();

    position.y += verticalVelocity * GetFrameTime();
    if(position.y > ground){
        position.y = ground;
        verticalVelocity = 0.0f;
        onGround = true;
    }

    // Animation
    if(onGround){
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
        currentFrame = 0;
        frameRec.x = (float)currentFrame*(float)ryan.width/6;
        velocity.y += 1;
    }
}

Rectangle Player::GetRect(){
    return Rectangle{position.x, position.y, float(ryan.width/6), float(ryan.height)};
}

void Player::DrawHitbox(bool isColliding){
    Color outlineColor = isColliding ? RED : GREEN;
    DrawRectangleLinesEx(GetRect(), 3, outlineColor);
}
