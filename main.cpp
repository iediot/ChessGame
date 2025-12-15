#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

#include "piece_moves.h"

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    int resolution = 1000;

    auto window = SDL_CreateWindow("Chess", 100, 100, resolution, resolution, 0);
    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

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
    std::pair<int, int> white_king_pos = {7, 4};
    std::pair<int, int> black_king_pos = {0, 4};
    struct castling {
        bool white_can_castle_kingside  = true;
        bool white_can_castle_queenside = true;
        bool black_can_castle_kingside  = true;
        bool black_can_castle_queenside = true;
    } castling;
    bool is_white_checked = false;
    bool is_black_checked = false;
    bool en_passant_able = false;
    bool en_passant_is_white;
    int en_passant_row;
    int en_passant_col;

    bool running = true;
    while (running) {
        SDL_Event e;

        if (white_king_pos.first != 7 || white_king_pos.second != 4) {
            castling.white_can_castle_kingside = false;
            castling.white_can_castle_queenside = false;
        }
        if (black_king_pos.first != 0 || black_king_pos.second != 4) {
            castling.black_can_castle_kingside = false;
            castling.black_can_castle_queenside = false;
        }

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
                        auto prev_white_king = white_king_pos;
                        auto prev_black_king = black_king_pos;
                        int captured_piece = board[row][col];

                        board[selected_row][selected_col] = 0;
                        board[row][col] = piece;

                        if (std::abs(piece) == 6) {
                            if (white_turn)
                                white_king_pos = {row, col};
                            else
                                black_king_pos = {row, col};
                        }

                        if (std::abs(piece) == 1 && en_passant_able)
                        {
                            if (piece > 0 && !en_passant_is_white) {
                                if (row == (en_passant_row - 1))
                                    board[en_passant_row][en_passant_col] = 0;
                            } else if (piece < 0 && en_passant_is_white) {
                                if (row == (en_passant_row + 1))
                                    board[en_passant_row][en_passant_col] = 0;
                            }
                        }

                        bool mover_in_check;
                        if (white_turn) {
                            mover_in_check = is_square_attacked(board,
                                white_king_pos.first, white_king_pos.second, false);
                        } else {
                            mover_in_check = is_square_attacked(board,
                                black_king_pos.first, black_king_pos.second, true);
                        }

                        if (mover_in_check) {
                            board[selected_row][selected_col] = piece;
                            board[row][col] = captured_piece;
                            white_king_pos = prev_white_king;
                            black_king_pos = prev_black_king;
                        } else {
                            white_turn = !white_turn;

                            if (board[0][0] != -2 && castling.black_can_castle_queenside)
                                castling.black_can_castle_queenside = false;
                            if (board[0][7] != -2 && castling.black_can_castle_kingside)
                                castling.black_can_castle_kingside = false;
                            if (board[7][0] != 2 && castling.white_can_castle_queenside)
                                castling.white_can_castle_queenside = false;
                            if (board[7][7] != 2 && castling.white_can_castle_kingside)
                                castling.white_can_castle_kingside = false;

                            is_white_checked = is_square_attacked(board,
                                white_king_pos.first, white_king_pos.second, false);
                            is_black_checked = is_square_attacked(board,
                                black_king_pos.first, black_king_pos.second, true);
                        }
                    }

                    if (std::abs(piece) == 6 && std::abs(col - selected_col) == 2) {
                        if (col == 6) {
                            board[row][5] = board[row][7];
                            board[row][7] = 0;
                        } else if (col == 2) {
                            board[row][3] = board[row][0];
                            board[row][0] = 0;
                        }
                    }

                    if (std::abs(piece) == 1 && std::abs(row - selected_row) == 2) {
                        en_passant_able = true;
                        en_passant_row = row;
                        en_passant_col = col;
                        en_passant_is_white = piece > 0;
                    } else if (en_passant_is_white == white_turn) {
                        en_passant_able = false;
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
                        drag_x = e.button.x;
                        drag_y = e.button.y;
                        valid_moves = getValidMoves(board, row, col);
                        if (std::abs(piece) == 6)
                        {
                            if (piece > 0) {
                                if (can_castle(board, true, true,
                                castling.white_can_castle_queenside, false))
                                    valid_moves.emplace_back(7, 2);
                                if (can_castle(board, true, false,
                                false, castling.white_can_castle_kingside))
                                    valid_moves.emplace_back(7, 6);
                            } else {
                                if (can_castle(board, false, true,
                                castling.black_can_castle_queenside, false))
                                    valid_moves.emplace_back(0, 2);
                                if (can_castle(board, false, false,
                                false, castling.black_can_castle_kingside))
                                    valid_moves.emplace_back(0, 6);
                            }
                        }

                        if (std::abs(piece) == 1 && en_passant_able)
                        {
                            if (piece > 0 && !en_passant_is_white) {
                                if (selected_row == en_passant_row &&
                                    std::abs(selected_col - en_passant_col) == 1)
                                    valid_moves.emplace_back(selected_row - 1, en_passant_col);
                            } else if (piece < 0 && en_passant_is_white) {
                                if (selected_row == en_passant_row &&
                                    std::abs(selected_col - en_passant_col) == 1)
                                    valid_moves.emplace_back(selected_row + 1, en_passant_col);
                            }
                        }
                    }
                }
            }

            if (e.type == SDL_MOUSEMOTION && piece != 0) {
                if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(1)) {
                    drag_x = e.motion.x;
                    drag_y = e.motion.y;
                    dragging = true;
                }
            }

            if (e.type == SDL_MOUSEBUTTONUP) {
                int drop_col = e.button.x / cell_size;
                int drop_row = e.button.y / cell_size;

                if (dragging && piece != 0) {
                    bool isValid = false;
                    for (auto& move : valid_moves) {
                        if (move.first == drop_row && move.second == drop_col) {
                            isValid = true;
                            break;
                        }
                    }

                    if (isValid) {
                        auto prev_white_king = white_king_pos;
                        auto prev_black_king = black_king_pos;
                        int captured_piece = board[drop_row][drop_col];

                        board[selected_row][selected_col] = 0;
                        board[drop_row][drop_col] = piece;

                        if (std::abs(piece) == 6) {
                            if (white_turn)
                                white_king_pos = {drop_row, drop_col};
                            else
                                black_king_pos = {drop_row, drop_col};
                        }

                        if (std::abs(piece) == 1 && en_passant_able)
                        {
                            if (piece > 0 && !en_passant_is_white) {
                                if (drop_row == (en_passant_row - 1))
                                    board[en_passant_row][en_passant_col] = 0;
                            } else if (piece < 0 && en_passant_is_white) {
                                if (drop_row == (en_passant_row + 1))
                                    board[en_passant_row][en_passant_col] = 0;
                            }
                        }

                        bool mover_in_check;
                        if (white_turn) {
                            mover_in_check = is_square_attacked(board,
                                white_king_pos.first, white_king_pos.second, false);
                        } else {
                            mover_in_check = is_square_attacked(board,
                                black_king_pos.first, black_king_pos.second, true);
                        }

                        if (mover_in_check) {
                            board[selected_row][selected_col] = piece;
                            board[drop_row][drop_col] = captured_piece;
                            white_king_pos = prev_white_king;
                            black_king_pos = prev_black_king;
                        } else {
                            white_turn = !white_turn;

                            if (board[0][0] != -2 && castling.black_can_castle_queenside)
                                castling.black_can_castle_queenside = false;
                            if (board[0][7] != -2 && castling.black_can_castle_kingside)
                                castling.black_can_castle_kingside = false;
                            if (board[7][0] != 2 && castling.white_can_castle_queenside)
                                castling.white_can_castle_queenside = false;
                            if (board[7][7] != 2 && castling.white_can_castle_kingside)
                                castling.white_can_castle_kingside = false;

                            is_white_checked = is_square_attacked(board,
                                white_king_pos.first, white_king_pos.second, false);
                            is_black_checked = is_square_attacked(board,
                                black_king_pos.first, black_king_pos.second, true);
                        }
                    }

                    if (std::abs(piece) == 6 && std::abs(drop_col - selected_col) == 2) {
                        if (drop_col == 6) {
                            board[drop_row][5] = board[drop_row][7];
                            board[drop_row][7] = 0;
                        } else if (drop_col == 2) {
                            board[drop_row][3] = board[drop_row][0];
                            board[drop_row][0] = 0;
                        }
                    }

                    if (std::abs(piece) == 1 && en_passant_able)
                    {
                        if (piece > 0 && !en_passant_is_white) {
                            if (selected_row == en_passant_row &&
                                std::abs(selected_col - en_passant_col) == 1)
                                valid_moves.emplace_back(selected_row - 1, en_passant_col);
                        } else if (piece < 0 && en_passant_is_white) {
                            if (selected_row == en_passant_row &&
                                std::abs(selected_col - en_passant_col) == 1)
                                valid_moves.emplace_back(selected_row + 1, en_passant_col);
                        }
                    }

                    if (std::abs(piece) == 1 && std::abs(drop_row - selected_row) == 2) {
                        en_passant_able = true;
                        en_passant_row = drop_row;
                        en_passant_col = drop_col;
                        en_passant_is_white = piece > 0;
                    } else  if (en_passant_is_white == white_turn) {
                        en_passant_able = false;
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

                if ((is_white_checked && row == white_king_pos.first && col == white_king_pos.second) ||
                    (is_black_checked && row == black_king_pos.first && col == black_king_pos.second)) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
                    SDL_RenderFillRect(renderer, &square);
    }

                if (!(dragging && row == selected_row && col == selected_col)) {
                    if (getTexture(board[row][col])) {
                        SDL_RenderCopy(renderer, getTexture(board[row][col]), nullptr, &square);
                    }
                }
            }
        }

        for (auto& move :  valid_moves) {
            int center_x = move.second * cell_size + cell_size / 2;
            int center_y = move.first * cell_size + cell_size / 2;

            if (board[move.first][move.second] == 0) {
                filledCircleRGBA(renderer, center_x, center_y, cell_size / 6, 0, 0, 0, 80);
            } else {
                filledCircleRGBA(renderer, center_x, center_y, cell_size / 2 - 2, 0, 0, 0, 80);
            }
        }

        if (dragging && piece != 0) {
            SDL_Rect dragRect = {drag_x - cell_size/2, drag_y - cell_size/2, cell_size, cell_size};
            SDL_RenderCopy(renderer, getTexture(piece), nullptr, &dragRect);
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