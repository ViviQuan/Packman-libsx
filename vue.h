#pragma once

#include <libsx.h>
#include "modele.h"

extern GameState *g_game_state; // Pointeur d'état global du jeu

extern void init_gui(GameState *state); // Initialiser l'interface graphique
extern void update_gui(GameState *state); // Mettre à jour l'interface graphique

// Dessiner le plateau de jeu
extern void draw_board(Widget w, int width, int height, void *data);
extern void show_help_dialog(void); // Afficher la boîte de dialogue d'aide
extern void show_game_over(void); // Afficher la boîte de dialogue de fin de partie
