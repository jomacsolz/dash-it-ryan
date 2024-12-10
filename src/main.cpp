#include <raylib.h>
#include <vector>
#include "ryan.hpp"
#include "bg.hpp"
#include "obstacle.hpp"
#include "powerup.hpp"

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

    int score = 0;
    int highScore = 0;
    float scoreTimer = 0.0f;
    const float scoreInterval = 0.1f;

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
    
    SetTargetFPS(60);

    BG bg;
    Player ryan;
    ObstacleSpawn bugs;
    Texture2D bugTex = LoadTexture("assets/obstacle.png");
    PowerupSpawn power;

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
                    score++;
                    scoreTimer = 0.0f;

                    if(score % 100 == 0){
                        bg.SpeedUp(1.0f);
                        bugs.SpeedUp(0.1f);
                    }

                    if(score % 100 == 0){
                        int rando = rand() % powerupTex.size();
                        Powerup newPowerup(powerupTex[rando], bugs.getSpeed());
                        power.getPowerups().push_back(newPowerup);
                    }
                }
            } break;
            case ENDING:
            {
                if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_TAB)){ // return to title screen
                    currentScreen = TITLE;
                    score = 0; // reset score
                    bg.resetSpeed(); // reset bg speed
                    bugs.resetSpeed(); // reset obstacle speed
                    bugs = ObstacleSpawn(); // reset obstacles
                }else if(IsKeyPressed(KEY_R)){ // restart game
                    currentScreen = GAMEPLAY;
                    score = 0;
                    bg.resetSpeed();
                    bugs.resetSpeed();
                    bugs = ObstacleSpawn();
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

                    // Player Draw
                    ryan.Draw();

                    // Obstacle Draw
                    bugs.Update(bugTex, power.powerupActive());
                    bugs.Draw();

                    // Powerup Draw
                    power.Update();
                    power.Draw();

                    // score display
                    DrawText(TextFormat("Score: %i", score), screenWidth-150, 10, 20, WHITE);

                    if(CheckPowerCollision(ryan, power.getPowerups())){
                        // add powerup effects here
                    }
                    
                    if(CheckCollision(ryan, bugs.getObstacles())){
                        currentScreen = ENDING;
                        PlaySound(deathSFX);
                        StopMusicStream(gameBGM);
                    }
                } break;
                case ENDING:
                {
                    if(score > highScore) highScore = score;

                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawTextureEx(menu, (Vector2){0,0}, 0.0f, scaleImg(menu), WHITE); // bg image
                    DrawTexture(end, 0, 0, WHITE);

                    textWidth = MeasureText(TextFormat("Score: %i", score), 40);
                    DrawText(TextFormat("Score: %i", score), (screenWidth - textWidth) / 2, (screenHeight - 40) / 2 + 40, 40, WHITE);

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
    UnloadTexture(bugTex);
    UnloadSound(deathSFX);
    UnloadMusicStream(gameBGM);
    UnloadMusicStream(menuBGM);
    
    CloseAudioDevice();
    CloseWindow();
    return 0;
}