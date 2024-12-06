#include "bg.hpp"

BG::BG(){
    background = LoadTexture("assets/bg/1.png");
    midground = LoadTexture("assets/bg/2.png");
    foreground = LoadTexture("assets/bg/3.png");
    scrollBack = 0.0f;
    scrollMid = 0.0f;
    scrollFore = 0.0f;
    bgSpeed = 1.0f;
}

BG::~BG(){
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
}

void BG::Draw(){
    DrawTextureEx(background, (Vector2){scrollBack, 0}, 0.0f, 7.0f, WHITE);
    DrawTextureEx(background, (Vector2){background.width*7 + scrollBack, 0}, 0.0f, 7.0f, WHITE);

    DrawTextureEx(midground, (Vector2){scrollMid, 0}, 0.0f, 7.0f, WHITE);
    DrawTextureEx(midground, (Vector2){background.width*7 + scrollMid, 0}, 0.0f, 7.0f, WHITE);

    DrawTextureEx(foreground, (Vector2){scrollFore, 0}, 0.0f, 7.0f, WHITE);
    DrawTextureEx(foreground, (Vector2){background.width*7 + scrollFore, 0}, 0.0f, 7.0f, WHITE);
}

void BG::Update(){
    scrollBack -= 0.1f * bgSpeed;
    scrollMid -= 0.5f * bgSpeed;
    scrollFore -= 1.0f * bgSpeed;

    if(scrollBack <= -background.width*7) scrollBack = 0;
    if(scrollMid <= -midground.width*7) scrollMid = 0;
    if(scrollFore <= -foreground.width*7) scrollFore = 0;
}

void BG::SpeedUp(float x){
    bgSpeed += x;
    std::cout << "Background speed increased: " << bgSpeed << std::endl;
}

void BG::resetSpeed(){
    bgSpeed = 1.0f;
    std::cout << "Background speed reset: " << bgSpeed << std::endl;
}