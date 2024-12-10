#include "powerup.hpp"

Powerup::Powerup(Texture2D powerTex, float x){
    texture = powerTex;
    position = {(float)GetScreenWidth(), (float)(3*720)/4 + texture.height/2};
    speed = 8.0f;
    speedMulti = x;

    // Animation variables
    currentFrame = 0;
    framesCounter = 0;
    framesSpeed = 6;
    frameRec = {0.0f, 0.0f, (float)texture.width/6, (float)texture.height};
}

Powerup::~Powerup(){
    //UnloadTexture(texture);
}

void Powerup::InitPowerup(float x){
    position = {(float)GetScreenWidth(), (float)(3*720)/4 + texture.height/2};
    speed = 8.0f;
    speedMulti = x;
}

void Powerup::Draw(){
    DrawTextureRec(texture, frameRec, position, WHITE);
}

void Powerup::Update(){
    position.x -= speed * speedMulti;

    // Animation
    framesCounter++;
    if(framesCounter >= (60 / framesSpeed)){
        framesCounter = 0;
        currentFrame++;

        if(currentFrame > 5)
            currentFrame = 0;
        frameRec.x = (float)currentFrame*(float)texture.width/6;
    }
}

Rectangle Powerup::GetRect(){
    return Rectangle{position.x, position.y, (float)texture.width/6, (float)texture.height};
}

void Powerup::resetSpeed(){
    speedMulti = 1.0f;
}

float Powerup::getSpeed(){
    return speedMulti;
}

void Powerup::setSpeed(float x){
    speedMulti = x;
}

PowerupSpawn::PowerupSpawn(){
    spawnTimer = 0.0f;
    spawnInterval = 1.0f;
    globalSpeed = 1.0f;
}

void PowerupSpawn::Update(bool& obsSpawned, float& obsSpawnTimer, float obsSpawnDelay){
    if(obsSpawned){
        obsSpawnTimer += GetFrameTime();
        if(obsSpawnTimer >= obsSpawnDelay)
            obsSpawned = false;
        return; // skip spawn
    }

    for(auto& powerup : powerups)
        powerup.Update();

    powerups.erase(std::remove_if(powerups.begin(), powerups.end(), [](Powerup& pu){
        return pu.GetRect().x < -pu.GetRect().width;
    }), powerups.end());
}

void PowerupSpawn::Draw(){
    // if(!powerups.empty())
    //     powerups.front().Draw();
    for(auto& powerup : powerups)
        powerup.Draw();
}

void PowerupSpawn::resetSpeed(){
    globalSpeed = 1.0f;
    // if(!powerups.empty())
    //     powerups.front().resetSpeed();
    for(auto& powerup : powerups)
        powerup.resetSpeed();
}

std::vector<Powerup>& PowerupSpawn::getPowerups(){
    return powerups;
}

bool PowerupSpawn::powerupActive(){
    return !powerups.empty();
}

float PowerupSpawn::getSpeed(){
    if(!powerups.empty()){
        return powerups[0].getSpeed();
    }
    else
        return 1.0f;
}

void PowerupSpawn::setSpeed(float x){
    // if(!powerups.empty())
    //     powerups.front().setSpeed(x);
    for(auto& powerup : powerups)
        powerup.setSpeed(x);
}