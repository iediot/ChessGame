#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    int resolution;
    std:: cout << "Enter resolution size (1x1): ";
    std::cin >> resolution;

    auto window = SDL_CreateWindow("Chess", 100, 100, resolution, resolution, 0);
    auto renderer = SDL_CreateRenderer(window, -1, 0);

    int cellSize = resolution / 8;

    SDL_Texture* whitePawn = IMG_LoadTexture(renderer, "assets/white_pawn.png");
    SDL_Texture* blackPawn = IMG_LoadTexture(renderer, "assets/black_pawn.png");
    SDL_Texture* whiteRook = IMG_LoadTexture(renderer, "assets/white_rook.png");
    SDL_Texture* blackRook = IMG_LoadTexture(renderer, "assets/black_rook.png");
    SDL_Texture* whiteKnight = IMG_LoadTexture(renderer, "assets/white_knight.png");
    SDL_Texture* blackKnight = IMG_LoadTexture(renderer, "assets/black_knight.png");
    SDL_Texture* whiteBishop = IMG_LoadTexture(renderer, "assets/white_bishop.png");
    SDL_Texture* blackBishop = IMG_LoadTexture(renderer, "assets/black_bishop.png");
    SDL_Texture* whiteQueen = IMG_LoadTexture(renderer, "assets/white_queen.png");
    SDL_Texture* blackQueen = IMG_LoadTexture(renderer, "assets/black_queen.png");
    SDL_Texture* whiteKing = IMG_LoadTexture(renderer, "assets/white_king.png");
    SDL_Texture* blackKing = IMG_LoadTexture(renderer, "assets/black_king.png");

    int board[8][8] = {
        {-2, -3, -4, -5, -6, -4, -3, -2},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        { 0,  0,  0,  0,  0,  0,  0,  0},
        { 0,  0,  0,  0,  0,  0,  0,  0},
        { 0,  0,  0,  0,  0,  0,  0,  0},
        { 0,  0,  0,  0,  0,  0,  0,  0},
        { 1,  1,  1,  1,  1,  1,  1,  1},
        { 2,  3,  4,  5,  6,  4,  3,  2}
    };

    auto getTexture = [&](int piece) -> SDL_Texture* {
        switch (piece) {
            case  1: return whitePawn;
            case -1: return blackPawn;
            case  2: return whiteRook;
            case -2: return blackRook;
            case  3: return whiteKnight;
            case -3: return blackKnight;
            case  4: return whiteBishop;
            case -4: return blackBishop;
            case  5: return whiteQueen;
            case -5: return blackQueen;
            case  6: return whiteKing;
            case -6: return blackKing;
            default: return nullptr;
        }
    };

    int selectedRow = -1, selectedCol = -1;
    int dragX = -1, dragY = -1;
    int piece = 0;
    bool dragging = false;

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int col = e.button.x / cellSize;
                int row = e. button.y / cellSize;

                if (piece != 0) {
                    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
                        board[selectedRow][selectedCol] = 0;
                        board[row][col] = piece;
                    }
                    piece = 0;
                    selectedRow = -1;
                    selectedCol = -1;
                } else if (board[row][col] != 0) {
                    selectedRow = row;
                    selectedCol = col;
                    piece = board[row][col];
                    dragX = e.button.x;
                    dragY = e.button.y;
                }
            }

            if (e.type == SDL_MOUSEMOTION && piece != 0) {
                if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1)) {
                    dragX = e.motion.x;
                    dragY = e.motion.y;
                    dragging = true;
                }
            }

            if (e.type == SDL_MOUSEBUTTONUP) {
                int dropCol = e.button.x / cellSize;
                int dropRow = e.button.y / cellSize;

                if (dragging && piece != 0 && (dropRow != selectedRow || dropCol != selectedCol)) {
                    if (dropRow >= 0 && dropRow < 8 && dropCol >= 0 && dropCol < 8) {
                        board[selectedRow][selectedCol] = 0;
                        board[dropRow][dropCol] = piece;
                    }
                    piece = 0;
                    selectedRow = -1;
                    selectedCol = -1;
                }
                dragging = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                SDL_Rect square = {col * cellSize, row * cellSize, cellSize, cellSize};

                if ((row + col) % 2 == 1) {
                    SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);
                    SDL_RenderFillRect(renderer, &square);
                }

                if (row == selectedRow && col == selectedCol && !dragging) {
                    SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                    SDL_RenderFillRect(renderer, &square);
                }

                if (!(dragging && row == selectedRow && col == selectedCol)) {
                    SDL_Texture* tex = getTexture(board[row][col]);
                    if (tex) {
                        SDL_RenderCopy(renderer, tex, NULL, &square);
                    }
                }
            }
        }

        if (dragging && piece != 0) {
            SDL_Rect dragRect = {dragX - cellSize/2, dragY - cellSize/2, cellSize, cellSize};
            SDL_RenderCopy(renderer, getTexture(piece), NULL, &dragRect);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(whitePawn);
    SDL_DestroyTexture(blackPawn);
    SDL_DestroyTexture(whiteRook);
    SDL_DestroyTexture(blackRook);
    SDL_DestroyTexture(whiteKnight);
    SDL_DestroyTexture(blackKnight);
    SDL_DestroyTexture(whiteBishop);
    SDL_DestroyTexture(blackBishop);
    SDL_DestroyTexture(whiteQueen);
    SDL_DestroyTexture(blackQueen);
    SDL_DestroyTexture(whiteKing);
    SDL_DestroyTexture(blackKing);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}