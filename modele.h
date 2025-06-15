#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Définir le type d'énumération de la direction de déplacement
typedef enum{
  UP,
  DOWN,
  LEFT,
  RIGHT
} Direction;

// Structure de l'état du jeu
typedef struct{
  int rows;
  int cols;
  int **board;
  int pacman_x;
  int pacman_y;
  int move_count;
  int eaten_count;
  int total_pawns;
  int total_cells;
  Direction dir;
} GameState;

extern void init_game_state(GameState *,int ,int );
extern void free_game_state(GameState *);
extern void place_pawns_randomly(GameState *);
extern void move_pacman(GameState *,Direction );
extern void move_pacman_randomly(GameState *);
extern void move_pacman_zigzag(GameState *);
extern void move_pacman_dfs(GameState *);
extern int has_pawn_to_eat(GameState *);
extern int get_move_count(GameState *);
extern int get_eaten_count(GameState *);
