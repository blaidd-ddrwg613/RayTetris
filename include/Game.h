#pragma once
#include <array>
#include <random>

#include "GameBoard.h"

class Game
{
public:
    Game();
    void Update();
    void Draw() const;

private:
    // Where the board sits on screen. Recomputed every frame from the window
    // size, so the board scales and stays centered when the window is resized.
    struct Layout
    {
        int boardX;     // top-left pixel of the board
        int boardY;
        int cellSize;   // size of one cell in pixels
    };
    Layout ComputeLayout() const;

    // 7-bag randomizer. Declared before the pieces, because the constructor
    // uses it to create them, and members are built in declaration order.
    std::mt19937 _rng{ std::random_device{}() };
    std::array<PieceTypes::PieceType, PieceTypes::PIECE_COUNT> _bag{};
    int _bagIndex = PieceTypes::PIECE_COUNT;   // "empty", so the first call fills it

    PieceTypes::PieceType NextFromBag();

    Tetromino _currentTetromino;
    Tetromino _nextTetromino;

    GameBoard _gameBoard;

    // Gravity timer (same idea as the Snake tick)
    float _fallTimer = 0.0f;
    float _fallInterval = 0.5f;   // seconds per row; shrinks as level rises

    int _score = 0;
    int _linesCleared = 0;
    int _level = 0;

    bool _gameOver = false;

    void HandleInput();
    bool TryMove(int dx, int dy); // try-on-a-copy using _gameBoard.Fits()
    void HardDrop();
    bool TryRotate(int dir);
    void LockPiece();             // PlacePiece, ClearLines, score, spawn next
    void SpawnNext();             // next becomes current, make a new next, check game over
    void Reset();

    void DrawPreview(PieceTypes::PieceType type, int boxX, int boxY, int cellSize) const;
    void DrawTetromino(const Tetromino& piece, Color color, int originX, int originY, int cellSize) const;
    void DrawScore(const Layout& layout) const;
};
