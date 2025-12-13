#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

#include "piece_moves.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    int resolution = 1000;

    auto window = SDL_CreateWindow("Chess", 100, 100, resolution, resolution, 0);
    auto renderer = SDL_CreateRenderer(window, -1, 0);

    int cell_size = resolution / 8;

    SDL_Texture* white_pawn = IMG_LoadTexture(renderer, "assets/white_pawn.png");
    SDL_Texture* black_pawn = IMG_LoadTexture(renderer, "assets/black_pawn.png");
    SDL_Texture* white_rook = IMG_LoadTexture(renderer, "assets/white_rook.png");
    SDL_Texture* black_rook = IMG_LoadTexture(renderer, "assets/black_rook.png");
    SDL_Texture* white_knight = IMG_LoadTexture(renderer, "assets/white_knight.png");
    SDL_Texture* black_knight = IMG_LoadTexture(renderer, "assets/black_knight.png");
    SDL_Texture* white_bishop = IMG_LoadTexture(renderer, "assets/white_bishop.png");
    SDL_Texture* black_bishop = IMG_LoadTexture(renderer, "assets/black_bishop.png");
    SDL_Texture* white_queen = IMG_LoadTexture(renderer, "assets/white_queen.png");
    SDL_Texture* black_queen = IMG_LoadTexture(renderer, "assets/black_queen.png");
    SDL_Texture* white_king = IMG_LoadTexture(renderer, "assets/white_king.png");
    SDL_Texture* black_king = IMG_LoadTexture(renderer, "assets/black_king.png");

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
            case  1: return white_pawn;
            case -1: return black_pawn;
            case  2: return white_rook;
            case -2: return black_rook;
            case  3: return white_knight;
            case -3: return black_knight;
            case  4: return white_bishop;
            case -4: return black_bishop;
            case  5: return white_queen;
            case -5: return black_queen;
            case  6: return white_king;
            case -6: return black_king;
            default: return nullptr;
        }
    };

    int selected_row = -1, selected_col = -1;
    int drag_x = -1, drag_y = -1;
    int piece = 0;
    bool dragging = false;
    bool white_turn = true;
    std::vector<std::pair<int, int>> valid_moves;

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int col = e.button.x / cell_size;
                int row = e.button.y / cell_size;

                if (piece != 0) {
                    bool isValid = false;
                    for (auto& move : valid_moves) {
                        if (move.first == row && move.second == col) {
                            isValid = true;
                            break;
                        }
                    }

                    if (isValid) {
                        board[selected_row][selected_col] = 0;
                        board[row][col] = piece;
                        white_turn = !white_turn;
                    }
                    piece = 0;
                    selected_row = -1;
                    selected_col = -1;
                    valid_moves.clear();

                } else if (board[row][col] != 0) {
                    bool is_white_piece = board[row][col] > 0;

                    if (is_white_piece == white_turn) {
                        selected_row = row;
                        selected_col = col;
                        piece = board[row][col];
                        drag_x = e.button. x;
                        drag_y = e.button.y;
                        valid_moves = getValidMoves(board, row, col);
                    }
                }
            }

            if (e.type == SDL_MOUSEMOTION && piece != 0) {
                if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1)) {
                    drag_x = e.motion.x;
                    drag_y = e.motion.y;
                    dragging = true;
                }
            }

            if (e.type == SDL_MOUSEBUTTONUP) {
                int drop_col = e.button.x / cell_size;
                int drop_row = e. button.y / cell_size;

                if (dragging && piece != 0) {
                    bool isValid = false;
                    for (auto& move : valid_moves) {
                        if (move.first == drop_row && move.second == drop_col) {
                            isValid = true;
                            break;
                        }
                    }

                    if (isValid) {
                        board[selected_row][selected_col] = 0;
                        board[drop_row][drop_col] = piece;
                        white_turn = !white_turn;
                    }
                    piece = 0;
                    selected_row = -1;
                    selected_col = -1;
                    valid_moves.clear();
                }
                dragging = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                SDL_Rect square = {col * cell_size, row * cell_size, cell_size, cell_size};

                if ((row + col) % 2 == 1) {
                    SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);
                    SDL_RenderFillRect(renderer, &square);
                }

                if (row == selected_row && col == selected_col && !dragging) {
                    SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                    SDL_RenderFillRect(renderer, &square);
                }

                if (!(dragging && row == selected_row && col == selected_col)) {
                    SDL_Texture* tex = getTexture(board[row][col]);
                    if (tex) {
                        SDL_RenderCopy(renderer, tex, NULL, &square);
                    }
                }
            }
        }

        for (auto& move :  valid_moves) {
            int centerX = move.second * cell_size + cell_size / 2;
            int centerY = move. first * cell_size + cell_size / 2;

            if (board[move.first][move.second] == 0) {
                filledCircleRGBA(renderer, centerX, centerY, cell_size / 6, 0, 0, 0, 80);
            } else {
                filledCircleRGBA(renderer, centerX, centerY, cell_size / 2 - 2, 0, 0, 0, 80);
            }
        }

        if (dragging && piece != 0) {
            SDL_Rect dragRect = {drag_x - cell_size/2, drag_y - cell_size/2, cell_size, cell_size};
            SDL_RenderCopy(renderer, getTexture(piece), NULL, &dragRect);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(white_pawn);
    SDL_DestroyTexture(black_pawn);
    SDL_DestroyTexture(white_rook);
    SDL_DestroyTexture(black_rook);
    SDL_DestroyTexture(white_knight);
    SDL_DestroyTexture(black_knight);
    SDL_DestroyTexture(white_bishop);
    SDL_DestroyTexture(black_bishop);
    SDL_DestroyTexture(white_queen);
    SDL_DestroyTexture(black_queen);
    SDL_DestroyTexture(white_king);
    SDL_DestroyTexture(black_king);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}