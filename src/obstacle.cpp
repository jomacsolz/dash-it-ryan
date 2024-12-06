#include "obstacle.hpp"

Obstacle::Obstacle(){
    // texture = LoadTexture("assets/hitbox.png");
    hitbox.width = 50;
    hitbox.height = 50;
    hitbox.x = GetScreenWidth();
    hitbox.y = (3*720)/4 + hitbox.height;
    speed = 8.0f;
    speedMulti = 1.0f;
}

Obstacle::~Obstacle(){
    UnloadTexture(texture);
}

void Obstacle::InitObstacle(float multiplier){
    hitbox.width = 50;
    hitbox.height = 50;
    hitbox.x = GetScreenWidth();
    hitbox.y = (3*720)/4 + hitbox.height;
    speed = 8.0f;
    speedMulti = multiplier;
}

void Obstacle::Draw(){
    // Draw obstacle texture here
}

void Obstacle::Update(){
    hitbox.x -= speed * speedMulti;
    DrawRectangleLinesEx(hitbox, 5, RED);
}

Rectangle Obstacle::GetRect(){
    return hitbox;
}

void Obstacle::SpeedUp(float x){
    speedMulti += x;
    std::cout << "Obstacle speed multiplier increased: " << speedMulti << std::endl;
}

void Obstacle::resetSpeed(){
    speedMulti = 1.0f;
    std::cout << "Obstacle speed multiplier reset: " << speedMulti << std::endl;
}

ObstacleSpawn::ObstacleSpawn(){
    spawnTimer = 0.0f;
    spawnInterval = 1.0f; // Initial spawn interval
    globalSpeed = 1.0f;
}

void ObstacleSpawn::Update(){
    spawnTimer += GetFrameTime();
    if(spawnTimer >= spawnInterval && obstacles.size() < 4){
        Obstacle newObstacle;
        newObstacle.InitObstacle(globalSpeed);
        obstacles.push_back(newObstacle);
        spawnTimer = 0.0f;
        spawnInterval = 1.0f + (rand() % 2);
    }

    for(auto& obstacle : obstacles){
        obstacle.Update();
    }

    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](Obstacle& ob){
        return ob.GetRect().x < -ob.GetRect().width;
    }), obstacles.end());
}

void ObstacleSpawn::Draw(){
    for(auto& obstacle : obstacles)
        obstacle.Draw();
}

void ObstacleSpawn::SpeedUp(float x){
    globalSpeed += x;
    for(auto& obstacle : obstacles)
        obstacle.SpeedUp(x);
}

void ObstacleSpawn::resetSpeed(){
    globalSpeed = 1.0f;
    for(auto& obstacle : obstacles)
        obstacle.resetSpeed();
}

std::vector<Obstacle> &ObstacleSpawn::getObstacles(){
    return obstacles;
}
