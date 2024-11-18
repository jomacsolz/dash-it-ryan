#include <raylib.h>
#include "ryan.hpp"

int main() {
    InitWindow(1280, 720, "Run It Ryan");
    SetTargetFPS(60);

    Player ryan;

    while(WindowShouldClose() == false){
        ryan.Update();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        ryan.Draw();
        EndDrawing();
    }
    
    CloseWindow();
}