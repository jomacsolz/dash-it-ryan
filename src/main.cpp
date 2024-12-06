#include <raylib.h>
#include "ryan.hpp"
#include "bg.hpp"
#include "obstacle.hpp"

#define NUM_FRAMES 3

const int screenWidth = 1280;
const int screenHeight = 720;

bool CheckCollision(Player& player, std::vector<Obstacle>& obstacles){
    for(auto& obstacle: obstacles){
        if(CheckCollisionRecs(player.GetRect(), obstacle.GetRect()))
            return true;
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
    float scoreTimer = 0.0f;
    const float scoreInterval = 0.1f;

    // button
    Texture2D button = LoadTexture("assets/playButton.png");
    float frameHeight = (float)button.height/NUM_FRAMES;
    Rectangle sourceRec = {0, 0, (float)button.width, frameHeight};
    Rectangle btnBounds = {screenWidth/2.0f - button.width/2.0f, screenHeight/2.0f - button.height/NUM_FRAMES/2.0f, (float)button.width, frameHeight};
    int btnState = 0;
    bool btnAction = false;
    Vector2 mousePoint = {0.0f, 0.0f};

    // Title screen textures
    Texture2D logo = LoadTexture("assets/menu/logo.png");
    Texture2D menu = LoadTexture("assets/menu/flat.png");

    float scale = 0.5f;
    float posX = (screenWidth - logo.width * scale) / 2;
    float posY = (screenHeight - logo.height * scale) / 2 - 200;

    // SFX
    Music gameBGM = LoadMusicStream("assets/audio/ingame.wav");
    Music menuBGM = LoadMusicStream("assets/audio/menu.wav");
    Sound deathSFX = LoadSound("assets/audio/death.wav");
    
    SetTargetFPS(60);

    BG bg;
    Player ryan;
    ObstacleSpawn obby;

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
                    currentScreen = GAMEPLAY;
                    StopMusicStream(menuBGM);
                    PlayMusicStream(gameBGM);
                }
                mousePoint = GetMousePosition();
                btnAction = false;

                if(CheckCollisionPointRec(mousePoint, btnBounds)){
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
                    else btnState = 0;

                    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
                }else btnState = 0;

                if(btnAction){
                    // PlaySound(fxButton); // button sfx
                    currentScreen = GAMEPLAY;
                    StopMusicStream(menuBGM);
                    PlayMusicStream(gameBGM);
                }

                sourceRec.y = btnState*frameHeight;
            } break;
            case GAMEPLAY:
            {
                // if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_TAB))
                //     currentScreen = ENDING;
            } break;
            case ENDING:
            {
                if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_TAB)){
                    currentScreen = TITLE;
                    score = 0; // reset score
                    bg.resetSpeed(); // reset bg speed
                    obby.resetSpeed(); // reset obstacle speed
                    obby = ObstacleSpawn(); // re-initialize obstacle
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

                    DrawTextureRec(button, sourceRec, (Vector2){btnBounds.x, btnBounds.y}, WHITE);
                } break;
                case GAMEPLAY:
                {
                    // Score counter
                    scoreTimer += GetFrameTime();
                    if(scoreTimer >= scoreInterval){
                        score++;
                        scoreTimer = 0.0f;

                        if(score % 50 == 0){
                            bg.SpeedUp(1.0f);
                            obby.SpeedUp(0.1f);
                        }
                    }

                    // Draw Background
                    ClearBackground(RAYWHITE);
                    bg.Draw();
                    

                    // Obstacle Draw
                    obby.Update();


                    // Player Draw
                    ryan.Draw();
                    // ryan.DrawHitbox(false);

                    // score display
                    DrawText(TextFormat("Score: %i", score), screenWidth-150, 10, 20, WHITE);

                    if(CheckCollision(ryan, obby.getObstacles())){
                        currentScreen = ENDING;
                        PlaySound(deathSFX);
                        StopMusicStream(gameBGM);
                    }
                } break;
                case ENDING:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    textWidth = MeasureText("Game Over", 40);
                    DrawText("Game Over", (screenWidth - textWidth) / 2, (screenHeight - 40) / 2, 40, RED);
                    textWidth = MeasureText(TextFormat("Score: %i", score), 20);
                    DrawText(TextFormat("Score: %i", score), (screenWidth - textWidth) / 2, (screenHeight - 40) / 2 + 40, 20, WHITE);
                    textWidth = MeasureText("Press Enter or Tab to return to title screen", 20);
                    DrawText("Press Enter or Tab to return to title screen", (screenWidth - textWidth) / 2, (screenHeight - 20) / 2 + 60, 20, GRAY);
                } break;
                default: break;
            }
            
        EndDrawing();
    }

    UnloadTexture(button);
    UnloadTexture(logo);
    UnloadTexture(menu);
    UnloadSound(deathSFX);
    UnloadMusicStream(gameBGM);
    UnloadMusicStream(menuBGM);
    
    CloseAudioDevice();
    CloseWindow();
    return 0;
}