#pragma once
#include <raylib.h>
#include <vector>
#include <algorithm>

class Powerup{
private:
    Texture2D texture;
    Vector2 position;
    float speed;
    float speedMulti;

    // animation variables
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    Rectangle frameRec;

public:
    Powerup(Texture2D, float);
    ~Powerup();
    void InitPowerup(float);
    void Draw();
    void Update();
    Rectangle GetRect();
    void resetSpeed();
};

class PowerupSpawn{
private:
    std::vector<Powerup> powerups;
    float spawnTimer;
    float spawnInterval;
    float globalSpeed;
    bool active;

public:
    PowerupSpawn();
    void Update();
    void Draw();
    void resetSpeed();
    std::vector<Powerup>& getPowerups();
    bool powerupActive();
};