#include "obstacle.hpp"

Obstacle::Obstacle(Texture2D texture, bool fly){
    isFlying = fly;
    obs = texture;
    position = {(float)GetScreenWidth(), (float)(3*720)/4 + obs.height/2};
    speed = 8.0f;
    speedMulti = 1.0f;
    
}

Obstacle::~Obstacle(){
    
}

void Obstacle::InitObstacle(float multiplier){
    if(isFlying)
        position = {(float)GetScreenWidth(), (float)(3*720)/4 + obs.height/2 - 120};
    else
        position = {(float)GetScreenWidth(), (float)(3*720)/4 + obs.height/2};
    
    speed = 8.0f;
    speedMulti = multiplier;
}

void Obstacle::Draw(){
    DrawTexture(obs, position.x, position.y, WHITE);
}

void Obstacle::Update(bool slow, Texture2D texture){
    if(slow)
        position.x -= 5.0f;
    else
        position.x -= speed * speedMulti;
    //DrawRectangleLinesEx(GetRect(), 1, RED); // display hitbox
}

Rectangle Obstacle::GetRect(){
    return Rectangle{position.x, position.y, (float)(obs.width), (float)obs.height};
}

void Obstacle::SpeedUp(float x){
    speedMulti += x;
    std::cout << "Obstacle speed multiplier increased: " << speedMulti << std::endl;
}

void Obstacle::SpeedDown(){
    speedMulti -= 1.5f;
}

void Obstacle::resetSpeed(){
    speedMulti = 1.0f;
    std::cout << "Obstacle speed multiplier reset: " << speedMulti << std::endl;
}

float Obstacle::getSpeed(){
    return speedMulti;
}

void Obstacle::setSpeed(float x){
    speedMulti = x;
}

ObstacleSpawn::ObstacleSpawn(){
    spawnDistance = 200.0f;
    globalSpeed = 1.0f;
}

void ObstacleSpawn::Update(Texture2D texture, bool power, bool slow, bool& obsSpawned, float& obsSpawnTimer, int index){
    if(!power && obstacles.size() < 3){
        if(obstacles.empty() || (GetScreenWidth() - obstacles.back().GetRect().x) >= spawnDistance){
            Obstacle newObstacle(texture, index == 6);
            newObstacle.InitObstacle(globalSpeed);
            obstacles.push_back(newObstacle);
            spawnDistance = 600.0f + rand() % 601;
            obsSpawned = true;
            obsSpawnTimer = 0.0f;
        }
    }

    for(auto& obstacle : obstacles){
        obstacle.Update(slow, texture);
    }

    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](Obstacle& ob){
        return ob.GetRect().x < -ob.GetRect().width;
    }), obstacles.end());
}

void ObstacleSpawn::Draw(){
    for(auto& obstacle : obstacles){
        obstacle.Draw();
    }
}

void ObstacleSpawn::SpeedUp(float x){
    globalSpeed += x;
    for(auto& obstacle : obstacles)
        obstacle.SpeedUp(x);
}

void ObstacleSpawn::SpeedDown(){
    globalSpeed -= 1.5f;
    for(auto& obstacle : obstacles)
        obstacle.SpeedDown();
}

void ObstacleSpawn::resetSpeed(){
    globalSpeed = 1.0f;
    for(auto& obstacle : obstacles)
        obstacle.resetSpeed();
}

std::vector<Obstacle> &ObstacleSpawn::getObstacles(){
    return obstacles;
}

float ObstacleSpawn::getSpeed(){
    if(!obstacles.empty()){
        return obstacles[0].getSpeed();
    }
    return 1.0f;
}

void ObstacleSpawn::setSpeed(float x){
    for(auto& obstacle : obstacles)
        obstacle.setSpeed(x);
}