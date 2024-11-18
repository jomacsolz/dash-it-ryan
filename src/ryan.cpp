#include "ryan.hpp"

Player::Player(){
    image = LoadTexture("assets/dino.png");
    position.x = 100;
    position.y = 100;
}

Player::~Player(){
    UnloadTexture(image);
}

void Player::Draw(){
    DrawTextureV(image, position, WHITE);
}