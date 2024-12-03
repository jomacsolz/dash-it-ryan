#include <raylib.h>
#include "ryan.hpp"
#include "bg.hpp"
#include "obstacle.hpp"

bool CheckCollision(Player &player, Obstacle &obstacle){
    return CheckCollisionRecs(player.GetRect(), obstacle.GetRect());
}

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Dash It Ryan");

    GameScreen currentScreen = LOGO;

    int fpsCounter = 0;
    
    SetTargetFPS(60);

    BG bg;
    Player ryan;
    Obstacle obby;

    while(!WindowShouldClose()){
        switch(currentScreen){
            case LOGO:
            {
                fpsCounter++;
                if(fpsCounter > 60)
                    currentScreen = TITLE;
            } break;
            case TITLE:
            {
                if(IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                    currentScreen = GAMEPLAY;
            } break;
            case GAMEPLAY:
            {
                if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_TAB))
                    currentScreen = ENDING;
            } break;
            case ENDING:
            {
                if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_TAB)){
                    currentScreen = TITLE;
                    obby.InitObstacle(); // re-initialize obstacle
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
                    textWidth = MeasureText("LOGO HERE", 40);
                    DrawText("LOGO HERE", (screenWidth - textWidth) / 2, (screenHeight - 40) / 2, 40, LIGHTGRAY);
                    textWidth = MeasureText("loading...", 20);
                    DrawText("loading...", (screenWidth - textWidth) / 2, (screenHeight - 20) / 2 + 40, 20, GRAY);
                } break;
                case TITLE:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    textWidth = MeasureText("Dash It Ryan", 40);
                    DrawText("Dash It Ryan", (screenWidth - textWidth) / 2, (screenHeight - 40) / 2, 40, LIGHTGRAY);
                    textWidth = MeasureText("Press Enter or Tap to Start", 20);
                    DrawText("Press Enter or Tap to Start", (screenWidth - textWidth) / 2, (screenHeight - 20) / 2 + 40, 20, GRAY);
                } break;
                case GAMEPLAY:
                {
                    // Draw Background
                    ClearBackground(RAYWHITE);
                    bg.Draw();
                    

                    // Obstacle Draw
                    obby.Update();


                    // Player Draw
                    ryan.Draw();
                    ryan.DrawHitbox(false);

                    if(CheckCollision(ryan, obby)){
                        currentScreen = ENDING;
                    }
                } break;
                case ENDING:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    textWidth = MeasureText("Game Over", 40);
                    DrawText("Game Over", (screenWidth - textWidth) / 2, (screenHeight - 40) / 2, 40, LIGHTGRAY);
                    textWidth = MeasureText("Press Enter or Tab to Restart", 20);
                    DrawText("Press Enter or Tab to Restart", (screenWidth - textWidth) / 2, (screenHeight - 20) / 2 + 40, 20, GRAY);
                } break;
                default: break;
            }
            
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}