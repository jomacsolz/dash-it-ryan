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
    Obstacle(Texture2D);
    ~Obstacle();
    void InitObstacle(float);
    void Draw();
    void Update();
    Rectangle GetRect();
    Texture2D getTexture(){return obs;}

    // speed functions
    void SpeedUp(float);
    void resetSpeed();
    float getSpeed();
};

class ObstacleSpawn{
private:
    std::vector<Obstacle> obstacles;
    float spawnDistance;
    float globalSpeed;

public:
    ObstacleSpawn();
    void Update(Texture2D, bool);
    void Draw();
    void SpeedUp(float);
    void resetSpeed();
    std::vector<Obstacle>& getObstacles();
    float getSpeed();
};