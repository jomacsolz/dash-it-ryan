#include <iostream>
#include "raylib.h"

int main(){
    InitWindow(1280, 720, "Run It Ryan");
    SetTargetFPS(60);

    while(WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Run It Ryan", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    return 0;
}