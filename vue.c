#include <libsx.h>
#include "modele.h"
#include "callback.h"
#include "vue.h"

// Définition de variable de composant global
Widget widget_draw_area;
Widget widget_up_btn;
Widget widget_down_btn;
Widget widget_left_btn;
Widget widget_right_btn;
Widget widget_random_btn;
Widget widget_zigzag_btn;
Widget widget_dfs_btn;
Widget widget_restart_btn;
Widget widget_quit_btn;
Widget widget_help_btn;
Widget widget_pions_label;
Widget widget_moves_label;
Widget widget_pions_value;
Widget widget_moves_value;

GameState *g_game_state = NULL; // Pointeur d'état global du jeu
int cell_size = 50; // Taille de la cellule, en pixels

// Initialiser l'interface graphique, créer et configurer tous les composants de l'interface utilisateur
void init_gui(GameState *state) {
    // Enregistrer l'état du jeu dans une variable globale
    g_game_state = state;
    
    // Créer une zone de dessin à gauche, la taille est calculée en fonction de la taille du plateau de jeu
    widget_draw_area = MakeDrawArea(cell_size*state->cols, cell_size*state->rows, draw_area_callback, state);

    // Créer tous les boutons de contrôle
    widget_up_btn = MakeButton("Up", up_button_callback, NULL);
    widget_down_btn = MakeButton("Down", down_button_callback, NULL);
    widget_left_btn = MakeButton("Left", left_button_callback, NULL);
    widget_right_btn = MakeButton("Right", right_button_callback, NULL);
    widget_random_btn = MakeButton("Random", random_button_callback, NULL);
    widget_zigzag_btn = MakeButton("ZigZag", zigzag_button_callback, NULL);
    widget_dfs_btn = MakeButton("Dfs", dfs_button_callback, NULL);
    widget_restart_btn = MakeButton("Rejouer", restart_button_callback, NULL);
    widget_quit_btn = MakeButton("Quit", quit_button_callback, NULL);
    widget_help_btn = MakeButton("Aide", help_button_callback, NULL);
    
    // Créer une étiquette de compteur et sa valeur initiale
    char start[100];
    sprintf(start, "0/%d", state->total_pawns);
    widget_pions_label = MakeLabel("# Pions");
    widget_moves_label = MakeLabel("# Moves");
    widget_pions_value = MakeLabel(start);
    widget_moves_value = MakeLabel("0");

    // Définissez la taille uniforme de tous les composants à 120x60 pixels
    SetWidgetSize(widget_up_btn,120,60);
    SetWidgetSize(widget_down_btn,120,60);
    SetWidgetSize(widget_left_btn,120,60);
    SetWidgetSize(widget_right_btn,120,60);
    SetWidgetSize(widget_random_btn,120,60);
    SetWidgetSize(widget_zigzag_btn,120,60);
    SetWidgetSize(widget_dfs_btn,120,60);
    SetWidgetSize(widget_restart_btn,120,60);
    SetWidgetSize(widget_quit_btn,120,60);
    SetWidgetSize(widget_help_btn,120,60);
    SetWidgetSize(widget_pions_label,120,60);
    SetWidgetSize(widget_moves_label,120,60);
    SetWidgetSize(widget_pions_value,120,60);
    SetWidgetSize(widget_moves_value,120,60);
    
    // Disposition : les boutons de direction sont placés en haut
    SetWidgetPos(widget_up_btn, PLACE_RIGHT, widget_draw_area, NO_CARE, NULL);
    SetWidgetPos(widget_down_btn, PLACE_RIGHT, widget_up_btn, NO_CARE, NULL);
    SetWidgetPos(widget_left_btn, PLACE_RIGHT, widget_draw_area, PLACE_UNDER, widget_up_btn);
    SetWidgetPos(widget_right_btn, PLACE_RIGHT, widget_left_btn, PLACE_UNDER, widget_down_btn);
    
    // Disposition : Le bouton du mode automatique est placé sous les boutons de direction
    SetWidgetPos(widget_random_btn, PLACE_RIGHT, widget_draw_area, PLACE_UNDER, widget_left_btn);
    SetWidgetPos(widget_zigzag_btn, PLACE_RIGHT, widget_random_btn, PLACE_UNDER, widget_right_btn);
    SetWidgetPos(widget_dfs_btn, PLACE_RIGHT, widget_draw_area, PLACE_UNDER, widget_random_btn);
    SetWidgetPos(widget_restart_btn, PLACE_RIGHT, widget_dfs_btn, PLACE_UNDER, widget_zigzag_btn);
    
    // Disposition : L'affichage du compteur est placé sous le bouton du mode automatique
    SetWidgetPos(widget_pions_label, PLACE_RIGHT, widget_draw_area, PLACE_UNDER, widget_pions_value);
    SetWidgetPos(widget_pions_value, PLACE_RIGHT, widget_draw_area, PLACE_UNDER, widget_dfs_btn);
    SetWidgetPos(widget_moves_label, PLACE_RIGHT, widget_pions_label, PLACE_UNDER, widget_moves_value);
    SetWidgetPos(widget_moves_value, PLACE_RIGHT, widget_pions_value, PLACE_UNDER, widget_restart_btn);
    
    // Disposition : boutons Quitter et Aide en bas
    SetWidgetPos(widget_quit_btn, PLACE_RIGHT, widget_draw_area, PLACE_UNDER, widget_pions_label);
    SetWidgetPos(widget_help_btn, PLACE_RIGHT, widget_quit_btn, PLACE_UNDER, widget_moves_label);
    
    // Définir le rappel du bouton du clavier
    SetKeypressCB(widget_draw_area, key_press_callback);
    
    // Initialiser la couleur et afficher l'interface
    GetStandardColors();
    ShowDisplay();
}

// Dessinez le plateau de jeu, y compris la grille, Pac-Man et tous les haricots
void draw_board(Widget w, int width, int height, void *data) {
  GameState *state = (GameState *)data;

  // Effacez la zone de dessin et préparez-vous à redessiner
  ClearDrawArea();

  // Commencez par remplir toute la zone de noir
  SetFgColor(w,BLACK);
  DrawFilledBox(0, 0, cell_size*state->cols, cell_size*state->rows);

  // Dessinez les lignes de la grille
  SetFgColor(w,WHITE);
  for(int i=0; i<state->rows; i++) {
    for(int j=0; j<state->cols; j++) {
      DrawBox(j*cell_size, i*cell_size, cell_size, cell_size);
    }
  }

  // Calculer la position en pixels (point central) de Pac-Man
  int px=state->pacman_x*cell_size+cell_size/2;
  int py=state->pacman_y*cell_size+cell_size/2;
  SetFgColor(w, RED);
  // Déterminer l'angle auquel dessiner Pac-Man en fonction de la direction
  int arg1, arg2;
  switch(state->dir){
  case UP: arg1=120; arg2=300; break;
  case DOWN: arg1=300; arg2=300; break;
  case LEFT: arg1=210; arg2=300; break;
  case RIGHT: arg1=30; arg2=300; break;
  default:
    break;
  }
  // Dessinez Pac-Man (en utilisant un arc)
  DrawFilledArc(px-cell_size/2, py-cell_size/2, cell_size, cell_size, arg1, arg2);

  SetFgColor(w, BLUE);
  // Dessine tous les haricots
  for(int i=0; i<state->rows; i++) {
    for(int j=0; j<state->cols; j++) {
      if(state->board[i][j]==1) {
        int center_x = j*cell_size + cell_size/2;
        int center_y = i*cell_size + cell_size/2;
        
        DrawFilledArc(center_x - cell_size/6, center_y - cell_size/6, cell_size/3, cell_size/3, 0, 360);
      }
    }
  }
}

// Mettre à jour l'interface graphique, actualiser le compteur et l'affichage du plateau de jeu
void update_gui(GameState *state){
  if(!widget_pions_value || !widget_moves_value) return;

  // Mettre à jour l'affichage du compteur
  char pions[20],moves[20];
  sprintf(pions, "%d/%d", state->eaten_count, state->total_pawns);
  sprintf(moves, "%d", state->move_count);
  SetLabel(widget_pions_value, pions);
  SetLabel(widget_moves_value, moves);

  // Redessiner le plateau de jeu
  draw_board(widget_draw_area, 500, 500, state);

  // Affichage synchrone à l'écran
  SyncDisplay();
}

// Afficher la boîte de dialogue d'aide et afficher les instructions de fonctionnement du jeu
void show_help_dialog(void) {
  GetOkay("Game Instructions\n"
          "- Use arrow keys(W,A,S,D) or buttons to move Pacman\n"
          "- Eat all blue pawns (blue circles)\n"
          "- Modes: Random,Zigzag,Dfs\n"
	  "- Note: If you want to use the keyboard to control Pac-Man, the mouse cursor needs to be placed in the left box\n"
          "- Restart(key R) to reset game,Quit(key Q) to exit");
}

// Vérifie si la partie est terminée. Si tous les haricots sont mangés, affiche la boîte de dialogue de fin de partie.
void show_game_over(void){
  if(!has_pawn_to_eat(g_game_state)) {
    GetOkay("Game is over!");
  }
}
