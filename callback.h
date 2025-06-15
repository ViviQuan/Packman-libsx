#pragma once

#include <libsx.h>

extern void up_button_callback(Widget w, void *data);
extern void down_button_callback(Widget w, void *data);
extern void left_button_callback(Widget w, void *data);
extern void right_button_callback(Widget w, void *data);
extern void random_button_callback(Widget w, void *data);
extern void zigzag_button_callback(Widget w, void *data);
extern void dfs_button_callback(Widget w, void *data);
extern void restart_button_callback(Widget w, void *data);
extern void help_button_callback(Widget w, void *data);
extern void quit_button_callback(Widget w, void *data);

extern void key_press_callback(Widget w, char *input, int up_or_down, void *data);
extern void draw_area_callback(Widget w, int width, int height, void *data);
// Fonction de rappel de minuterie pour le mode de mouvement automatique
extern void timer_callback(void *data);

// Déclaration de variable globale
extern int timer_enabled; // Indicateur d'état de la minuterie activée
