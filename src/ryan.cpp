#include "ryan.hpp"

Player::Player(){
    image = LoadTexture("assets/dino.png");
    position.x = 100;
    position.y = 100;
    speed = 10;
}

Player::~Player(){
    UnloadTexture(image);
}

void Player::Draw(){
    DrawTextureV(image, position, WHITE);
}

void Player::Update(){
    if(IsKeyDown(KEY_RIGHT)) position.x += speed;
    if(IsKeyDown(KEY_LEFT)) position.x -= speed;
    if(IsKeyDown(KEY_UP)) position.y -= speed;
    if(IsKeyDown(KEY_DOWN)) position.y += speed;
}