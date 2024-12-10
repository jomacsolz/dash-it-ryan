#include <raylib.h>
#include <vector>
#include <queue>
#include "ryan.hpp"
#include "bg.hpp"
#include "obstacle.hpp"
#include "powerup.hpp"
#include <iostream>

#define NUM_FRAMES 2

const int screenWidth = 1280;
const int screenHeight = 720;

bool CheckCollision(Player& player, std::vector<Obstacle>& obstacles){
    for(auto& obstacle: obstacles){
        if(CheckCollisionRecs(player.GetRect(), obstacle.GetRect()))
            return true;
    }
    return false;
}

bool CheckPowerCollision(Player& player, std::vector<Powerup>& powerups){
    for(auto i = powerups.begin(); i != powerups.end(); i++){
        if(CheckCollisionRecs(player.GetRect(), i->GetRect())){
            powerups.erase(i);
            return true;
        }
    }
    return false;
}

float scaleImg(Texture2D img){
    float scaleX = (float)GetScreenWidth() / img.width;
    float scaleY = (float)GetScreenHeight() / img.height;
    return (scaleX > scaleY) ? scaleX : scaleY;
}

int main(){
    InitWindow(screenWidth, screenHeight, "Dash It Ryan");
    InitAudioDevice();

    GameScreen currentScreen = LOGO;
    int fpsCounter = 0;

    float score = 0;
    int highScore = 0;
    float scoreTimer = 0.0f;
    const float scoreInterval = 0.1f;
    float scoreMulti = 1.0f;
    float effectTimer = 0.0f;
    float effectDuration = 5.0f;

    bool hasShield = false;
    bool effectActive = false;
    bool isInvulnerable = false;
    bool hasTakenCoffee = false;
    bool isSlow = false;
    int getPower = -1;

    // Powerup notification
    std::string powerupMessage = "";
    float messageTimer = 0.0f;
    float messageDuration = 2.0f;

    // button
    Texture2D playButton = LoadTexture("assets/menu/play.png");
    Texture2D exitButton = LoadTexture("assets/menu/exit.png");
    float frameHeight = (float)playButton.height/NUM_FRAMES;
    Rectangle sourceRecPlay = {0, 0, (float)playButton.width, frameHeight};
    Rectangle btnBoundsPlay = {screenWidth/2.0f - playButton.width/2.0f, screenHeight/2.0f - playButton.height/NUM_FRAMES/2.0f + 150, (float)playButton.width, frameHeight};
    Rectangle sourceRecExit = {0, 0, (float)exitButton.width, frameHeight};
    Rectangle btnBoundsExit = {screenWidth/2.0f - exitButton.width/2.0f, screenHeight/2.0f - exitButton.height/NUM_FRAMES/2.0f + 250, (float)exitButton.width, frameHeight};
    int btnStatePlay = 0;
    bool btnActionPlay = false;
    int btnStateExit = 0;
    bool btnActionExit = false;
    Vector2 mousePoint = {0.0f, 0.0f};

    // Title screen textures
    Texture2D logo = LoadTexture("assets/menu/logo.png");
    Texture2D menu = LoadTexture("assets/menu/flat.png");
    Texture2D end = LoadTexture("assets/menu/end.png");
    Texture2D howToPlay0 = LoadTexture("assets/menu/how0.png");
    Texture2D howToPlay1 = LoadTexture("assets/menu/how1.png");

    float scale = 0.5f;
    float posX = (screenWidth - logo.width * scale) / 2;
    float posY = (screenHeight - logo.height * scale) / 2 - 100;

    // Audio
    Music gameBGM = LoadMusicStream("assets/audio/ingame.wav");
    Music menuBGM = LoadMusicStream("assets/audio/menu.wav");
    Sound deathSFX = LoadSound("assets/audio/death.wav");

    // Powerup Textures
    std::vector<Texture2D> powerupTex = {
        LoadTexture("assets/powerups/pu_1_5.png"),
        LoadTexture("assets/powerups/pu_2.png"),
        LoadTexture("assets/powerups/pu_3.png"),
        LoadTexture("assets/powerups/shield.png"),
        LoadTexture("assets/powerups/slow.png"),
        LoadTexture("assets/powerups/speed.png"),
        LoadTexture("assets/powerups/loop.png")
    };

    std::vector<Texture2D> bugTex = {
        LoadTexture("assets/obstacles/1.png"),
        LoadTexture("assets/obstacles/2.png"),
        LoadTexture("assets/obstacles/3.png"),
        LoadTexture("assets/obstacles/4.png"),
        LoadTexture("assets/obstacles/5.png"),
        LoadTexture("assets/obstacles/6.png"),
        LoadTexture("assets/obstacles/f2.png"),
    };
    
    SetTargetFPS(60);

    BG bg;
    Player ryan;
    ObstacleSpawn bugs;
    PowerupSpawn power;

    int rando;
    float bgSpeedTemp = bg.getSpeed();
    float obsSpeedTemp = bugs.getSpeed();
    float puSpeedTemp = power.getSpeed();

    bool obsSpawned = false;
    float obsSpawnDelay = 0.5f;
    float obsSpawnTimer = 0.0f;

    PlayMusicStream(menuBGM);

    // Main game loop
    while(!WindowShouldClose()){
        UpdateMusicStream(menuBGM);
        UpdateMusicStream(gameBGM);

        switch(currentScreen){
            case LOGO:
            {
                fpsCounter++;
                if(fpsCounter > 60)
                    currentScreen = TITLE;
            } break;
            case TITLE:
            {
                if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_TAB)){
                    currentScreen = INSTRUCTION1;
                }
                mousePoint = GetMousePosition();
                btnActionPlay = false;

                if(CheckCollisionPointRec(mousePoint, btnBoundsPlay)){
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnStatePlay = 0;
                    else btnStatePlay = 1;

                    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnActionPlay = true;
                }else btnStatePlay = 0;

                if(CheckCollisionPointRec(mousePoint, btnBoundsExit)){
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnStateExit = 0;
                    else btnStateExit = 1;

                    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnActionExit = true;
                }else btnStateExit = 0;

                if(btnActionPlay){
                    // PlaySound(fxButton); // button sfx
                    currentScreen = INSTRUCTION1;
                }
                if(btnActionExit){
                    CloseAudioDevice();
                    CloseWindow();
                    return 0;
                }

                sourceRecPlay.y = btnStatePlay*frameHeight;
                sourceRecExit.y = btnStateExit*frameHeight;
            } break;
            case INSTRUCTION1:
            {
                if(IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                    currentScreen = INSTRUCTION2;
                else if(IsKeyPressed(KEY_BACKSPACE))
                    currentScreen = TITLE;
            } break;
            case INSTRUCTION2:
            {
                if(IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)){
                    currentScreen = GAMEPLAY;
                    StopMusicStream(menuBGM);
                    PlayMusicStream(gameBGM);
                }else if(IsKeyPressed(KEY_BACKSPACE))
                    currentScreen = INSTRUCTION1;
            } break;
            case GAMEPLAY:
            {
                // Score counter
                scoreTimer += GetFrameTime();
                if(scoreTimer >= scoreInterval){
                    score += 1 * scoreMulti;
                    scoreTimer = 0.0f;

                    if((int)score % 100 == 0){
                        bg.SpeedUp(1.0f);
                        bugs.SpeedUp(0.1f);
                        bgSpeedTemp = bg.getSpeed();
                        std::cout << "bgSpeed: " << bgSpeedTemp << std::endl;
                        obsSpeedTemp = bugs.getSpeed();
                        std::cout << "obstacle Speed: " << obsSpeedTemp << std::endl;
                    }

                    if((int)score % 100 == 0){
                        rando = rand() % powerupTex.size();
                        Powerup newPowerup(powerupTex[rando], bugs.getSpeed());
                        puSpeedTemp = obsSpeedTemp;
                        std::cout << "powerup Speed: " << puSpeedTemp << std::endl;
                        power.getPowerups().push_back(newPowerup);
                    }
                }
            } break;
            case ENDING:
            {
                // reset any active powerups
                effectActive = false;
                scoreMulti = 1.0f;
                hasShield = false;
                isInvulnerable = false;
                if(hasTakenCoffee){
                    isInvulnerable = true;
                    effectDuration = 2.0f;
                    effectActive = true;
                    hasTakenCoffee = false;
                    bugs.SpeedDown();
                    bg.SpeedDown();
                }
                if(isSlow){
                    isSlow = false;
                    ryan.slowJump(isSlow);
                }

                if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_TAB)){ // return to title screen
                    currentScreen = TITLE;
                    score = 0; // reset score
                    bg.resetSpeed(); // reset bg speed
                    bugs.resetSpeed(); // reset obstacle speed
                    bugs = ObstacleSpawn(); // reset obstacles
                    power = PowerupSpawn(); // reset powerups
                    obsSpeedTemp = 1.0f; //reset obstacle speed temp var
                }else if(IsKeyPressed(KEY_R)){ // restart game
                    currentScreen = GAMEPLAY;
                    score = 0; // reset score
                    bg.resetSpeed(); // reset bg speed
                    bugs.resetSpeed(); // reset obstacle speed
                    bugs = ObstacleSpawn(); // reset obstacles
                    power = PowerupSpawn(); // reset powerups
                    obsSpeedTemp = 1.0f; //reset obstacle speed temp var
                    PlayMusicStream(gameBGM);
                }
            } break;
            default: break;
        }

        bg.Update();
        ryan.Update();

        BeginDrawing();

            int textWidth;
            switch(currentScreen){
                case LOGO:
                {
                    // logo image
                    DrawTextureEx(logo, (Vector2){posX,posY}, 0.0f, scale, WHITE);

                    textWidth = MeasureText("loading...", 20);
                    DrawText("loading...", (screenWidth - textWidth) / 2, (screenHeight - 20) / 2 + 200, 20, GRAY);
                } break;
                case TITLE:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);

                    DrawTextureEx(menu, (Vector2){0,0}, 0.0f, scaleImg(menu), WHITE); // bg image
                    DrawTextureEx(logo, (Vector2){posX,posY}, 0.0f, scale, WHITE); // logo image

                    DrawTextureRec(playButton, sourceRecPlay, (Vector2){btnBoundsPlay.x, btnBoundsPlay.y}, WHITE);
                    DrawTextureRec(exitButton, sourceRecExit, (Vector2){btnBoundsExit.x, btnBoundsExit.y}, WHITE);
                } break;
                case INSTRUCTION1:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawTextureEx(menu, (Vector2){0,0}, 0.0f, scaleImg(menu), WHITE); // bg image
                    DrawTextureEx(howToPlay0, (Vector2){0,0}, 0.0f, scaleImg(howToPlay0), WHITE);
                } break;
                case INSTRUCTION2:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawTextureEx(menu, (Vector2){0,0}, 0.0f, scaleImg(menu), WHITE); // bg image
                    DrawTextureEx(howToPlay1, (Vector2){0,0}, 0.0f, scaleImg(howToPlay0), WHITE);
                } break;
                case GAMEPLAY:
                {
                    // Draw Background
                    ClearBackground(RAYWHITE);
                    bg.Draw();

                    // Obstacle Draw
                    int obsRand = rand() % bugTex.size();
                    bugs.Update(bugTex[obsRand], power.powerupActive(), isSlow, obsSpawned, obsSpawnTimer, obsRand);
                    bugs.Draw();

                    // Player Draw
                    Color playerTint = effectActive ? SKYBLUE : WHITE;
                    ryan.Draw(playerTint);

                    // Powerup Draw
                    power.Update(obsSpawned, obsSpawnTimer, obsSpawnDelay);
                    power.Draw();

                    if(effectActive){
                        float barWidth = 200.0f;
                        float barHeight = 20.0f;
                        float barX = 40.0f;
                        float barY = 28.0f;
                        float remainingTime = effectDuration - effectTimer;
                        float barFillWidth = (remainingTime / effectDuration) * barWidth;

                        DrawRectangle(barX, barY, barWidth, barHeight, GRAY);
                        DrawRectangle(barX, barY, barFillWidth, barHeight, GREEN);
                    }

                    // score display
                    DrawText(TextFormat("Score: %i", (int)score), screenWidth-150, 10, 20, WHITE);

                    if(messageTimer > 0.0f){
                        messageTimer -= GetFrameTime();
                        int textWidth = MeasureText(powerupMessage.c_str(), 60);
                        DrawText(powerupMessage.c_str(), (screenWidth - textWidth) / 2, screenHeight / 2 - 200, 60, YELLOW);
                    }

                    if(CheckPowerCollision(ryan, power.getPowerups())){
                        // add powerup effects here
                        getPower = rando;
                        switch(getPower){
                            case 0:{
                                if(!effectActive){
                                    std::cout << "1.5x score multipler obtained!" << std::endl;
                                    scoreMulti *= 1.5f;
                                    effectTimer = 0.0f;
                                    effectDuration = 5.0f;
                                    effectActive = true;
                                    powerupMessage = "1.5x Stack Overflow!";
                                    messageTimer = messageDuration;
                                }
                                break;
                            }

                            case 1:{
                                if(!effectActive){
                                    std::cout << "2.0x score multipler obtained!" << std::endl;
                                    scoreMulti *= 2.0f;
                                    effectTimer = 0.0f;
                                    effectDuration = 5.0f;
                                    effectActive = true;
                                    powerupMessage = "2.0x Stack Overflow!";
                                    messageTimer = messageDuration;
                                }
                                break;
                            }

                            case 2:{
                                if(!effectActive){
                                    std::cout << "3.0x score multipler obtained!" << std::endl;
                                    scoreMulti *= 3.0f;
                                    effectTimer = 0.0f;
                                    effectDuration = 5.0f;
                                    effectActive = true;
                                    powerupMessage = "3.0x Stack Overflow!";
                                    messageTimer = messageDuration;
                                }
                                break;
                            }

                            case 3:{ // shield
                                if(!effectActive){
                                    std::cout << "Debug Shield obtained!" << std::endl;
                                    hasShield = true;
                                    effectTimer = 0.0f;
                                    effectDuration = 5.0f;
                                    effectActive = true;
                                    powerupMessage = "Debug Shield obtained!";
                                    messageTimer = messageDuration;
                                }
                                break;
                            }

                            case 4:{ // slow
                                if(!effectActive){
                                    std::cout << "Code Review obtained!" << std::endl;
                                    
                                    bugs.setSpeed(0.1f);
                                    std::cout << "Obstacle Speed updated to: " << bugs.getSpeed() << std::endl;
                                    bg.setSpeed(1.0f);
                                    std::cout << "BG Speed updated to: " << bg.getSpeed() << std::endl;
                                    power.setSpeed(0.1f);
                                    std::cout << "powerup Speed updated to: " << power.getSpeed() << std::endl;
                                    effectTimer = 0.0f;
                                    effectDuration = 5.0f;
                                    effectActive = true;
                                    isSlow = true;
                                    ryan.slowJump(isSlow);
                                    powerupMessage = "Code Review obtained!";
                                    messageTimer = messageDuration;
                                }
                                break;
                            }

                            case 5:{ // speed
                                if(!effectActive){
                                    std::cout << "Caffeine Rush obtained!" << std::endl;

                                    bugs.SpeedUp(1.5f);
                                    bg.SpeedUp(2.0f);

                                    effectTimer = 0.0f;
                                    effectDuration = 3.0f;
                                    hasTakenCoffee = true;
                                    isInvulnerable = true;
                                    effectActive = true;
                                    powerupMessage = "Caffeine Rush obtained!";
                                    messageTimer = messageDuration;
                                }
                                break;
                            }

                            case 6:{ // loop
                                if(!effectActive){
                                    std::cout << "Infinite Loop obtained!" << std::endl;
                                    effectTimer = 0.0f;
                                    effectDuration = 5.0f;
                                    isInvulnerable = true;
                                    effectActive = true;
                                    powerupMessage = "Infinite loop obtained!";
                                    messageTimer = messageDuration;
                                }
                                break;
                            }

                            default:
                                break;
                        }
                    }

                    if(effectActive){
                        effectTimer += GetFrameTime();
                        if(effectTimer >= effectDuration){
                            effectActive = false;
                            effectTimer = 0.0f;
                            effectDuration = 5.0f;

                            scoreMulti = 1.0f;
                            std::cout << "score multiplier reset!" << std::endl;
                            hasShield = false;
                            std::cout << "Shield expired!" << std::endl; 

                            if(hasTakenCoffee){
                                isInvulnerable = true;
                                effectDuration = 2.0f;
                                effectActive = true;
                                hasTakenCoffee = false;
                                bugs.SpeedDown();
                                bg.SpeedDown();
                            }else{
                                isInvulnerable = false;
                                std::cout << "Invulnerability expired!" << std::endl;
                            }
                            if(isSlow){
                                isSlow = false;
                                ryan.slowJump(isSlow);
                            }
                            bg.setSpeed(bgSpeedTemp);
                            std::cout << "Background speed set to: " << bgSpeedTemp << std::endl;
                            bugs.setSpeed(obsSpeedTemp);
                            std::cout << "Obstacle speed set to: " << obsSpeedTemp << std::endl;
                            power.setSpeed(puSpeedTemp);
                            std::cout << "Powerups speed set to: " << puSpeedTemp << std::endl;
                        }
                        Rectangle activePower = {0.0f, 0.0f, (float)powerupTex[getPower].width/6, (float)powerupTex[getPower].height};
                        DrawTextureRec(powerupTex[getPower], activePower, {10,10}, WHITE);
                    }
                    
                    if(CheckCollision(ryan, bugs.getObstacles())){
                        if(hasShield){
                            std::cout << "Shield consumed!" << std::endl;
                            isInvulnerable = true;
                            hasShield = false;
                            effectTimer = 0.0f;
                            effectDuration = 1.0f;
                            effectActive = false;
                        }else 
                        if(isInvulnerable){
                            std::cout << "Invulnerable!" << std::endl;   
                        }else{
                            std::cout << "DEAD" << std::endl;
                            currentScreen = ENDING;
                            PlaySound(deathSFX);
                            StopMusicStream(gameBGM);
                        }
                    }
                } break;
                case ENDING:
                {
                    if((int)score > highScore) highScore = (int)score;

                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawTextureEx(menu, (Vector2){0,0}, 0.0f, scaleImg(menu), WHITE); // bg image
                    DrawTexture(end, 0, 0, WHITE);

                    textWidth = MeasureText(TextFormat("Score: %i", (int)score), 40);
                    DrawText(TextFormat("Score: %i", (int)score), (screenWidth - textWidth) / 2, (screenHeight - 40) / 2 + 40, 40, WHITE);

                    textWidth = MeasureText(TextFormat("Highest score: %i", highScore), 20);
                    DrawText(TextFormat("Highest score: %i", highScore), (screenWidth - textWidth) / 2, (screenHeight - 20) / 2 + 80, 20, WHITE);

                    textWidth = MeasureText("Press Enter or Tab to return to title screen", 20);
                    DrawText("Press Enter or Tab to return to title screen", (screenWidth - textWidth) / 2, (screenHeight - 20) / 2 + 120, 20, GRAY);

                    textWidth = MeasureText("Press R to restart", 20);
                    DrawText("Press R to restart", (screenWidth - textWidth) / 2, (screenHeight - 20) / 2 + 140, 20, GRAY);
                } break;
                default: break;
            }
            
        EndDrawing();
    }

    UnloadTexture(playButton);
    UnloadTexture(logo);
    UnloadTexture(menu);
    // UnloadTexture(bugTex);
    UnloadSound(deathSFX);
    UnloadMusicStream(gameBGM);
    UnloadMusicStream(menuBGM);
    
    CloseAudioDevice();
    CloseWindow();
    return 0;
}