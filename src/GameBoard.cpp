#include "GameBoard.h"

namespace
{
    // Colors used only by the board itself.
    constexpr Color EMPTY_CELL_COLOR = { 30, 30, 30, 255 };
    constexpr Color BORDER_COLOR     = { 130, 130, 130, 255 };
}

bool GameBoard::IsInsideBoard(int x, int y) const
{
    return x >= 0 && x < BOARD_WIDTH &&
           y >= 0 && y < BOARD_HEIGHT;
}

// Assumes (x, y) is inside the board; check IsInsideBoard() first.
bool GameBoard::IsCellOccupied(int x, int y) const
{
    return board[y][x] != 0;
}

// True if every filled cell of the piece is on an empty cell of the board.
// Cells above the top of the board (y < 0) are allowed, as long as they are
// between the walls, so pieces can spawn and rotate partly off-screen.
bool GameBoard::Fits(const Tetromino& piece) const
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (!pieceCell(piece.GetType(), piece.GetRotation(), row, col))
                continue;

            int boardX = piece.GetX() + col;
            int boardY = piece.GetY() + row;

            if (boardY < 0)
            {
                // Above the visible board: only the side walls matter.
                if (boardX < 0 || boardX >= BOARD_WIDTH)
                    return false;
                continue;
            }

            if (!IsInsideBoard(boardX, boardY) || IsCellOccupied(boardX, boardY))
                return false;
        }
    }
    return true;
}

bool GameBoard::IsRowFull(int y) const
{
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        if (board[y][x] == 0)
            return false;
    }
    return true;
}

// Copies the piece's cells into the board. Call only after Fits() said the
// piece is in a legal spot. Cells above the top (y < 0) are dropped; the game
// treats a lock that high as game over.
void GameBoard::PlacePiece(const Tetromino& piece)
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (!pieceCell(piece.GetType(), piece.GetRotation(), row, col))
                continue;

            int boardX = piece.GetX() + col;
            int boardY = piece.GetY() + row;

            if (IsInsideBoard(boardX, boardY))
                board[boardY][boardX] = piece.GetType() + 1;
        }
    }
}

// Removes every full row and shifts everything above it down.
// Works bottom to top. After clearing a row, y is NOT decremented, because the
// row that just dropped into position y also needs checking.
int GameBoard::ClearLines()
{
    int cleared = 0;
    int y = BOARD_HEIGHT - 1;

    while (y >= 0)
    {
        if (IsRowFull(y))
        {
            // Shift every row above y down by one (whole rows, thanks to board[y][x]).
            for (int r = y; r > 0; r--)
                board[r] = board[r - 1];

            board[0].fill(0);   // new empty row at the top
            cleared++;
        }
        else
        {
            y--;
        }
    }
    return cleared;
}

void GameBoard::Reset()
{
    for (auto& row : board)
        row.fill(0);
}

void GameBoard::DrawCell(int gridX, int gridY, Color color, int originX, int originY, int cellSize)
{
    // One pixel smaller than the cell, so the background shows through as grid lines.
    DrawRectangle(originX + gridX * cellSize, originY + gridY * cellSize,cellSize - 1, cellSize - 1, color);
}

void GameBoard::Draw(int originX, int originY, int cellSize) const
{
    // Outline around the whole board, just outside the cells.
    DrawRectangleLines(originX - 2, originY - 2,BOARD_WIDTH * cellSize + 3, BOARD_HEIGHT * cellSize + 3, BORDER_COLOR);

    for (int y = 0; y < BOARD_HEIGHT; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            int cell = board[y][x];
            Color color = (cell == 0) ? EMPTY_CELL_COLOR : PIECE_COLORS[cell - 1];
            DrawCell(x, y, color, originX, originY, cellSize);
        }
    }
}