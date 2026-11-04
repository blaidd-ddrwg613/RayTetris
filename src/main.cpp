#include "Game.h"
#include "raylib.h"

int main()
{
    // Window flags must be set before the window is created.
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 640,"RayTetris");

    // Stop the window from being resized so small the board becomes unreadable.
    SetWindowMinSize(320, 360);

    Game game;

    while (!WindowShouldClose())
    {
        // Update
        game.Update();

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
