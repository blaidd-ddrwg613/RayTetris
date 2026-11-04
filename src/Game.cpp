#include "Game.h"

#include <algorithm>

namespace
{
    // Space around the board, measured in cells so it scales with the board.
    constexpr int MARGIN_CELLS     = 1;   // empty space on every edge
    constexpr int SIDE_PANEL_CELLS = 6;   // room right of the board for score / next piece
    constexpr int MIN_CELL_SIZE    = 8;   // never shrink cells below this
}

// _rng and _bag are declared before the pieces in Game.h, so they already
// exist when NextFromBag() runs here.
Game::Game() : _currentTetromino(NextFromBag()), _nextTetromino(NextFromBag())
{
}

void Game::Update()
{
    if (_gameOver)
    {
        if (IsKeyPressed(KEY_ENTER)) Reset();
        return;
    }

    HandleInput();

    _fallTimer += GetFrameTime();
    if (_fallTimer >= _fallInterval)
    {
        _fallTimer = 0.0f;
        if (!TryMove(0, 1))
            LockPiece();
    }
}

Game::Layout Game::ComputeLayout() const
{
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();

    // Total space the layout needs, in cells.
    const int cellsWide = MARGIN_CELLS + GameBoard::BOARD_WIDTH + SIDE_PANEL_CELLS + MARGIN_CELLS;
    const int cellsTall = MARGIN_CELLS + GameBoard::BOARD_HEIGHT + MARGIN_CELLS;

    // Biggest whole-pixel cell that fits in both directions.
    // Whole pixels keep the grid lines crisp.
    int cellSize = std::min(screenW / cellsWide, screenH / cellsTall);
    cellSize = std::max(cellSize, MIN_CELL_SIZE);

    // Center the whole layout (board + side panel) in the window.
    const int layoutW = cellsWide * cellSize;
    const int layoutH = cellsTall * cellSize;

    Layout layout;
    layout.cellSize = cellSize;
    layout.boardX   = (screenW - layoutW) / 2 + MARGIN_CELLS * cellSize;
    layout.boardY   = (screenH - layoutH) / 2 + MARGIN_CELLS * cellSize;
    return layout;
}

void Game::Draw() const
{
    const Layout layout = ComputeLayout();

    const int panelX   = layout.boardX + (GameBoard::BOARD_WIDTH + 1) * layout.cellSize;
    const int previewY = layout.boardY + layout.cellSize;   // one row down, under the label

    _gameBoard.Draw(layout.boardX, layout.boardY, layout.cellSize);

    // Ghost piece: where the current piece would land on a hard drop.
    // Drawn before the real piece so the real one is on top where they overlap.
    Tetromino ghost = _currentTetromino;
    while (true)
    {
        Tetromino next = ghost;
        next.MoveDown();
        if (!_gameBoard.Fits(next)) break;
        ghost = next;
    }
    // DrawTetromino(ghost, Fade(PIECE_COLORS[ghost.GetType()], 0.3f),
    //               layout.boardX, layout.boardY, layout.cellSize);

    DrawTetromino(_currentTetromino, PIECE_COLORS[_currentTetromino.GetType()],
                  layout.boardX, layout.boardY, layout.cellSize);

    // "NEXT" label
    const int fontSize = layout.cellSize * 4 / 5;
    DrawText("NEXT", panelX, layout.boardY, fontSize, RAYWHITE);

    // Preview box background and outline
    const int boxSize = 4 * layout.cellSize;
    DrawRectangle(panelX, previewY, boxSize, boxSize, Color{ 30, 30, 30, 255 });
    DrawRectangleLines(panelX, previewY, boxSize, boxSize, GRAY);

    DrawPreview(_nextTetromino.GetType(), panelX, previewY, layout.cellSize);
    DrawScore(layout);

    // Drawn last so it sits on top of everything else.
    if (_gameOver)
    {
        const char* text = "Game Over! Press Enter to Start.";
        const int size = layout.cellSize;
        const int len  = MeasureText(text, size);
        const int y    = GetScreenHeight() / 2 - size / 2;

        DrawRectangle(0, y - size / 2, GetScreenWidth(), size * 2, Fade(BLACK, 0.8f));
        DrawText(text, GetScreenWidth() / 2 - len / 2, y, size, WHITE);
    }
}

// Draws a piece's shape (rotation 0) centered inside a 4x4-cell box.
void Game::DrawPreview(PieceTypes::PieceType type, int boxX, int boxY, int cellSize) const
{
    // Find the filled cells' bounding box within the 4x4 grid.
    int minRow = 4, maxRow = -1, minCol = 4, maxCol = -1;
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            if (pieceCell(type, 0, row, col))
            {
                minRow = std::min(minRow, row);  maxRow = std::max(maxRow, row);
                minCol = std::min(minCol, col);  maxCol = std::max(maxCol, col);
            }

    const int shapeW = maxCol - minCol + 1;
    const int shapeH = maxRow - minRow + 1;

    // Shift the origin so the shape lands in the middle of the box.
    const int originX = boxX + (4 - shapeW) * cellSize / 2 - minCol * cellSize;
    const int originY = boxY + (4 - shapeH) * cellSize / 2 - minRow * cellSize;

    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            if (pieceCell(type, 0, row, col))
                GameBoard::DrawCell(col, row, PIECE_COLORS[type], originX, originY, cellSize);
}

void Game::DrawTetromino(const Tetromino& piece, Color color, int originX, int originY, int cellSize) const
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (!pieceCell(piece.GetType(), piece.GetRotation(), row, col))
                continue;

            int gridX = piece.GetX() + col;
            int gridY = piece.GetY() + row;

            // Above the top of the board: don't draw over the margin.
            if (gridY < 0)
                continue;

            GameBoard::DrawCell(gridX, gridY, color, originX, originY, cellSize);
        }
    }
}

void Game::DrawScore(const Layout& layout) const
{
    const int panelX   = layout.boardX + (GameBoard::BOARD_WIDTH + 1) * layout.cellSize;
    const int fontSize = layout.cellSize * 4 / 5;
    const int lineGap  = fontSize + fontSize / 2;

    // Start below the NEXT label (1 row) and preview box (4 rows), plus a 1-row gap.
    int y = layout.boardY + 6 * layout.cellSize;

    DrawText(TextFormat("Score: %d", _score),        panelX, y, fontSize, WHITE);  y += lineGap;
    DrawText(TextFormat("Lines: %d", _linesCleared), panelX, y, fontSize, WHITE);  y += lineGap;
    DrawText(TextFormat("Level: %d", _level),        panelX, y, fontSize, WHITE);
}

void Game::HandleInput()
{
    // Hard drop first: it locks the piece and spawns a new one, so return
    // before any other key this frame can act on the new piece.
    if (IsKeyPressed(KEY_SPACE))
    {
        HardDrop();
        return;
    }

    if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) TryMove(-1, 0);
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) TryMove(+1, 0);
    if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) TryMove(0, +1);   // soft drop
    if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) TryRotate(+1);
}

bool Game::TryMove(int dx, int dy)
{
    Tetromino moved = _currentTetromino;
    if (dx < 0) moved.MoveLeft();
    if (dx > 0) moved.MoveRight();
    if (dy > 0) moved.MoveDown();

    if (!_gameBoard.Fits(moved))
        return false;

    _currentTetromino = moved;
    return true;
}

bool Game::TryRotate(int dir)
{
    Tetromino rotated = _currentTetromino;
    rotated.Rotate(dir);

    // Sideways nudges to try, in order: none, right, left, then 2 cells
    // (the I piece sometimes needs 2).
    constexpr int kicks[] = { 0, 1, -1, 2, -2 };

    for (int kick : kicks)
    {
        Tetromino attempt = rotated;
        for (int i = 0; i < kick; i++)  attempt.MoveRight();
        for (int i = 0; i > kick; i--)  attempt.MoveLeft();

        if (_gameBoard.Fits(attempt))
        {
            _currentTetromino = attempt;
            return true;
        }
    }
    return false;
}

void Game::LockPiece()
{
    _gameBoard.PlacePiece(_currentTetromino);
    int cleared = _gameBoard.ClearLines();
    if ( cleared != 0)
    {
        _linesCleared += cleared;

        switch (cleared)
        {
            case 1:
                _score += 40 * (_level + 1);
                break;
            case 2:
               _score += 100 * (_level + 1);
                break;
            case 3:
               _score += 300 * (_level + 1);
                break;
            case 4:
               _score += 1200 * (_level + 1);
                break;
            default:
                break;
        }

        // Level goes up every 10 lines, and pieces fall faster each level.
        _level = _linesCleared / 10;
        _fallInterval = std::max(0.05f, 0.5f - _level * 0.04f);
    }

    SpawnNext();
}

void Game::SpawnNext()
{
    _currentTetromino = _nextTetromino;
    _nextTetromino = Tetromino(NextFromBag());
    _fallTimer = 0.0f;   // give the new piece a full tick before it falls

    // No room for the new piece: game over.
    if (!_gameBoard.Fits(_currentTetromino))
        _gameOver = true;
}

void Game::HardDrop()
{
    int cellsDropped = 0;
    while (TryMove(0, 1))
        cellsDropped++;

    _score += cellsDropped * 2;   // optional: modern Tetris gives 2 points per cell
    LockPiece();
}

void Game::Reset()
{
    _bagIndex = PieceTypes::PIECE_COUNT;   // fresh bag for the new game
    _currentTetromino = Tetromino(NextFromBag());
    _nextTetromino    = Tetromino(NextFromBag());

    _fallTimer = 0.0f;
    _fallInterval = 0.5f;

    _score = 0;
    _linesCleared = 0;
    _level = 0;
    _gameOver = false;
    _gameBoard.Reset();
}

PieceTypes::PieceType Game::NextFromBag()
{
    if (_bagIndex >= PieceTypes::PIECE_COUNT)
    {
        // Refill with one of each piece, then shuffle.
        for (int i = 0; i < PieceTypes::PIECE_COUNT; i++)
            _bag[i] = static_cast<PieceTypes::PieceType>(i);

        std::shuffle(_bag.begin(), _bag.end(), _rng);
        _bagIndex = 0;
    }
    return _bag[_bagIndex++];
}