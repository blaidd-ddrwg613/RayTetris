#include "raylib.h"

int main()
{
    // Window flags must be set before the window is created.
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 640,"RayTetris");

    while (!WindowShouldClose())
    {
        // Update

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
