#include "bg.hpp"

BG::BG(){
    background = LoadTexture("assets/bg/1.png");
    midground = LoadTexture("assets/bg/2.png");
    midground2 = LoadTexture("assets/bg/2,5.png");
    foreground = LoadTexture("assets/bg/3.png");
    scrollBack = 0.0f;
    scrollMid = 0.0f;
    scrollMid2 = 0.0f;
    scrollFore = 0.0f;
    bgSpeed = 1.0f;
}

BG::~BG(){
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(midground2);
    UnloadTexture(foreground);
}

void BG::Draw(){
    DrawTextureEx(background, (Vector2){scrollBack, 0}, 0.0f, scaleImg(background), WHITE);
    DrawTextureEx(background, (Vector2){background.width*scaleImg(background) + scrollBack, 0}, 0.0f, scaleImg(background), WHITE);

    DrawTextureEx(midground, (Vector2){scrollMid, 0}, 0.0f, scaleImg(midground), WHITE);
    DrawTextureEx(midground, (Vector2){midground.width*scaleImg(background) + scrollMid, 0}, 0.0f, scaleImg(midground), WHITE);

    DrawTextureEx(midground2, (Vector2){scrollMid2, 0}, 0.0f, scaleImg(midground2), WHITE);
    DrawTextureEx(midground2, (Vector2){midground2.width*scaleImg(background) + scrollMid2, 0}, 0.0f, scaleImg(midground2), WHITE);

    DrawTextureEx(foreground, (Vector2){scrollFore, 0}, 0.0f, scaleImg(foreground), WHITE);
    DrawTextureEx(foreground, (Vector2){foreground.width*scaleImg(background) + scrollFore, 0}, 0.0f, scaleImg(foreground), WHITE);
}

void BG::Update(){
    scrollBack -= 0.1f * bgSpeed;
    scrollMid -= 0.5f * bgSpeed;
    scrollMid2 -= 0.8f * bgSpeed;
    scrollFore -= 1.0f * bgSpeed;

    float scale = scaleImg(background);
    if(scrollBack <= -background.width*scale) scrollBack = 0;
    if(scrollMid <= -midground.width*scale) scrollMid = 0;
    if(scrollMid2 <= -midground.width*scale) scrollMid2 = 0;
    if(scrollFore <= -foreground.width*scale) scrollFore = 0;
}

void BG::SpeedUp(float x){
    bgSpeed += x;
    std::cout << "Background speed increased: " << bgSpeed << std::endl;
}

void BG::resetSpeed(){
    bgSpeed = 1.0f;
    std::cout << "Background speed reset: " << bgSpeed << std::endl;
}

float BG::scaleImg(Texture2D img){
    float scaleX = (float)GetScreenWidth() / img.width;
    float scaleY = (float)GetScreenHeight() / img.height;
    return (scaleX > scaleY) ? scaleX : scaleY;
}