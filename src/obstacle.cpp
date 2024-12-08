#include "obstacle.hpp"

Obstacle::Obstacle(){
    obs = LoadTexture("assets/obstacle.png");
    position = {(float)GetScreenWidth(), (3*720)/4 + 50};
    speed = 8.0f;
    speedMulti = 1.0f;
}

Obstacle::~Obstacle(){
    UnloadTexture(obs);
}

void Obstacle::InitObstacle(float multiplier){
    position = {(float)GetScreenWidth(), (3*720)/4 + 50};
    speed = 8.0f;
    speedMulti = multiplier;
}

void Obstacle::Draw(){
}

void Obstacle::Update(){
    position.x -= speed * speedMulti;
    DrawRectangleLinesEx(GetRect(), 5, RED);
}

Rectangle Obstacle::GetRect(){
    return Rectangle{position.x, position.y, 50, 50};
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
    spawnInterval = 1.0f;
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
    for(auto& obstacle : obstacles){
        obstacle.Draw();
    }
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
