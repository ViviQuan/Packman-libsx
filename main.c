#include <libsx.h>
#include <stdio.h>
#include <stdlib.h>
#include "modele.h"
#include "vue.h"

int main(int argc, char *argv[]) {

  // Essayer d'initialiser et quitter en cas d'échec
  if(OpenDisplay(argc, argv)==0){
    fprintf(stderr,"Cannot open display\n");
    return EXIT_FAILURE;
  }

  // Obtenir la taille du plateau de jeu à partir des arguments de la ligne de commande
  int rows = atoi(argv[1]);
  int cols = atoi(argv[2]);

  // Créer l'état global du jeu
  g_game_state = (GameState*)malloc(sizeof(GameState));
  init_game_state(g_game_state, rows, cols);

  // Initialiser l'interface graphique
  init_gui(g_game_state);

  MainLoop();

  return EXIT_SUCCESS;
}
