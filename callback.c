#include <libsx.h>
#include <stdio.h>
#include <stdlib.h>
#include "callback.h"
#include "modele.h"
#include "vue.h"

// Indicateur d'état de la minuterie activée
int timer_enabled = 0;
// Mode de déplacement automatique actuel : -1 = désactiver, 0 = aléatoire, 1 = zigzag, 2 = DFS
int current_auto_mode = -1;

// Gérer l'événement de clic du bouton haut, déplacer Pac-Man vers le haut et mettre à jour l'interface
void up_button_callback(Widget w, void *data) {
  // Arrêter tous les modes automatiques
  timer_enabled = 0;
  current_auto_mode = -1;
  
  move_pacman(g_game_state, UP);
  update_gui(g_game_state);
  show_game_over();
}

// Gérer l'événement de clic du bouton vers le bas
void down_button_callback(Widget w, void *data) {
  timer_enabled = 0;
  current_auto_mode = -1;
  
  move_pacman(g_game_state, DOWN);
  update_gui(g_game_state);
  show_game_over();
}

// Gérer l'événement de clic du bouton gauche
void left_button_callback(Widget w, void *data) {
  timer_enabled = 0;
  current_auto_mode = -1;
  
  move_pacman(g_game_state, LEFT);
  update_gui(g_game_state);
  show_game_over();
}

// Gérer l'événement de clic droit
void right_button_callback(Widget w, void *data) {
  timer_enabled = 0;
  current_auto_mode = -1;
  
  move_pacman(g_game_state, RIGHT);
  update_gui(g_game_state);
  show_game_over();
}

// Gérer l'événement du bouton de déplacement aléatoire pour activer/désactiver le mode de déplacement aléatoire
void random_button_callback(Widget w, void *data) {
  // Si vous êtes déjà en mode aléatoire, désactivez-le
  if (timer_enabled && current_auto_mode == 0) {
    timer_enabled = 0;      // Arrêter le minuteur actuel
    current_auto_mode = -1; // Réinitialiser l'indicateur de mode automatique
  } else {
    // Sinon, activez le mode aléatoire
    timer_enabled = 0;
    current_auto_mode = 0;  // Définir en mode aléatoire
    timer_enabled = 1;
    // Définissez une minuterie de 300 millisecondes et utilisez la fonction timer_callback
    AddTimeOut(300, timer_callback, NULL);
  }
}

// Gérer l'événement du bouton de déplacement en forme de Z
void zigzag_button_callback(Widget w, void *data) {
  if (timer_enabled && current_auto_mode == 1) {
    timer_enabled = 0;      
    current_auto_mode = -1;
  } else {
    timer_enabled = 0;
    current_auto_mode = 1;  // Définir en mode zigzag
    timer_enabled = 1;
    AddTimeOut(300, timer_callback, NULL);
  }
}

// Gérer les événements du bouton de déplacement DFS
void dfs_button_callback(Widget w, void *data) {
  if (timer_enabled && current_auto_mode == 2) {
    timer_enabled = 0;      
    current_auto_mode = -1;
  } else {
    timer_enabled = 0;
    current_auto_mode = 2;  // Définir en mode DFS
    timer_enabled = 1;
    AddTimeOut(300, timer_callback, NULL);
  }
}

// Fonction de rappel de minuterie, effectue un mouvement automatique en fonction du mode actuel
void timer_callback(void *data) {
  if(!timer_enabled){
    return;
  }

  // Exécuter différentes stratégies de mouvement en fonction du mode automatique actuel
  switch(current_auto_mode) {
  case 0:
    move_pacman_randomly(g_game_state);
    break;
  case 1:
    move_pacman_zigzag(g_game_state);
    break;
  case 2:
    move_pacman_dfs(g_game_state);
    break;
  default:
    timer_enabled = 0; // Mode invalide, désactivez la minuterie
    return;
  }

  update_gui(g_game_state);

  if(!has_pawn_to_eat(g_game_state)) {
    timer_enabled = 0;
    current_auto_mode = -1;
    GetOkay("Game is over!");
  }
  else if(timer_enabled){
    // Si le jeu n'est pas terminé et que le minuteur est toujours activé, continuez à régler le minuteur suivant
    AddTimeOut(300, timer_callback, NULL);
  }
}

// Gérer l'événement du bouton de redémarrage et réinitialiser l'état du jeu
void restart_button_callback(Widget w, void *data) {
  // Arrêter tous les modes automatiques
  timer_enabled = 0;

  // Enregistrer la taille actuelle du plateau de jeu
  int rows = g_game_state->rows;
  int cols = g_game_state->cols;
  // Libérer l'état actuel du jeu
  free_game_state(g_game_state);
  init_game_state(g_game_state, rows, cols);

  update_gui(g_game_state);
}

// Gérer l'événement du bouton d'aide et afficher la boîte de dialogue des instructions du jeu
void help_button_callback(Widget w, void *data) {
  show_help_dialog();
}

// Gérer l'événement du bouton de sortie, libérer les ressources et quitter le programme
void quit_button_callback(Widget w, void *data) {
  free_game_state(g_game_state);
  exit(0);
}

// Gérer les événements des touches du clavier
void key_press_callback(Widget w, char *input, int up_or_down, void *data) {
  // S'il s'agit d'un événement de libération de clé, renvoyez directement
  if (up_or_down == 1) {  
    return;
  }

  // Arrêter tous les modes automatiques
  timer_enabled = 0;
  current_auto_mode = -1;
  
  // Effectuer les opérations correspondantes en fonction de la clé
  if (input[0] == 'w' || input[0] == 'W') {
    move_pacman(g_game_state, UP);
  } else if (input[0] == 's' || input[0] == 'S') {
    move_pacman(g_game_state, DOWN);
  } else if (input[0] == 'a' || input[0] == 'A') {
    move_pacman(g_game_state, LEFT);
  } else if (input[0] == 'd' || input[0] == 'D') {
    move_pacman(g_game_state, RIGHT);
  } else if (input[0] == 'q' || input[0] == 'Q') {
    exit(0);
  } else if (input[0] == 'r' || input[0] == 'R') {
    restart_button_callback(w, NULL);
  }
  
  update_gui(g_game_state);
  show_game_over();
}

// Fonction de rappel de zone de dessin
void draw_area_callback(Widget w, int width, int height, void *data) {
  draw_board(w, width, height, g_game_state);
}

