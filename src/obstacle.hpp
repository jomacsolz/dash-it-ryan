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
    bool isFlying;
public:
    Obstacle(Texture2D, bool);
    ~Obstacle();
    void InitObstacle(float);
    void Draw();
    void Update(bool, Texture2D);
    Rectangle GetRect();
    Texture2D getTexture(){return obs;}

    // speed functions
    void SpeedUp(float);
    void SpeedDown();
    void resetSpeed();
    float getSpeed();
    void setSpeed(float);
};

class ObstacleSpawn{
private:
    std::vector<Obstacle> obstacles;
    float spawnDistance;
    float globalSpeed;

public:
    ObstacleSpawn();
    void Update(Texture2D, bool, bool, bool&, float&, int);
    void Draw();
    void SpeedUp(float);
    void SpeedDown();
    void resetSpeed();
    std::vector<Obstacle>& getObstacles();
    float getSpeed();
    void setSpeed(float);
};
