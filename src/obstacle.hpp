#pragma once
#include <iostream>
#include <raylib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

class Obstacle{
private:
    Texture2D obs;
    Vector2 position;
    float speed;
    float speedMulti;
public:
    Obstacle();
    ~Obstacle();
    void InitObstacle(float);
    void Draw();
    void Update();
    Rectangle GetRect();
    Texture2D getTexture(){return obs;}

    // speed functions
    void SpeedUp(float);
    void resetSpeed();
};

class ObstacleSpawn{
private:
    std::vector<Obstacle> obstacles;
    float spawnTimer;
    float spawnInterval;
    float globalSpeed;

public:
    ObstacleSpawn();
    void Update();
    void Draw();
    void SpeedUp(float);
    void resetSpeed();
    std::vector<Obstacle>& getObstacles();
};