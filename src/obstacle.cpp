#include "obstacle.hpp"

Obstacle::Obstacle(){
    // obs = LoadTexture("assets/hitbox.png");
    hitbox.width = 50;
    hitbox.height = 50;
    hitbox.x = GetScreenWidth(); // Start at the right edge of the screen
    hitbox.y = (3*720)/4 + hitbox.height;
    speed = 8.0f;
}

Obstacle::~Obstacle(){
    UnloadTexture(obs);
}

void Obstacle::InitObstacle(){
    hitbox.width = 50;
    hitbox.height = 50;
    hitbox.x = GetScreenWidth(); // Start at the right edge of the screen
    hitbox.y = (3*720)/4 + hitbox.height;
    speed = 8.0f;
}

void Obstacle::Draw(){
    // Draw obstacle texture here
}

void Obstacle::Update(){
    hitbox.x -= speed;
    if(hitbox.x < -hitbox.width)
        InitObstacle();
    DrawRectangleLinesEx(hitbox, 5, RED);
}

Rectangle Obstacle::GetRect(){
    return Rectangle{hitbox.x, hitbox.y, hitbox.width, hitbox.height};
}