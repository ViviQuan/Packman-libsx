#include "modele.h"
#include <stdio.h>
#include <stdlib.h>

// Initialiser l'état du jeu, définir la taille du plateau de jeu et les paramètres initiaux
void init_game_state(GameState *state,int rows,int cols){

  // Initialisez la graine du nombre aléatoire une seule fois pour éviter de générer la même séquence lors d'appels répétés
  static int seed_initialized=0;
  if(!seed_initialized){
    srand(time(NULL));
    seed_initialized=1;
  }

  // Définir les paramètres de base du jeu
  state->rows=rows;
  state->cols=cols;
  state->move_count=0;
  state->eaten_count=0;
  state->dir=RIGHT;
  state->total_cells=rows*cols;

  // Allouer de la mémoire pour le plateau de jeu : allouer d'abord le tableau de pointeurs de ligne
  state->board = (int **)malloc(rows*sizeof(int *));
  for(int i=0;i<rows;i++){
    state->board[i]=(int *)calloc(cols,sizeof(int));
  }

  // Générer aléatoirement la position initiale de Pac-Man
  state->pacman_x=rand()%state->cols;
  state->pacman_y=rand()%state->rows;

  // Placer des haricots au hasard sur le plateau de jeu
  place_pawns_randomly(state);
}

// Libérer la mémoire d'état du jeu
void free_game_state(GameState *state){
  for(int i=0;i<state->rows;i++){
    free(state->board[i]);
  }
  free(state->board);
}

// Placez des haricots au hasard sur le plateau de jeu, représentant environ 20 % de la grille totale
void place_pawns_randomly(GameState *state){
  int total_cells=state->total_cells;
  int pawn_count=(int)(total_cells*0.2);
  state->total_pawns=pawn_count;

  while(pawn_count>0){
    int y=rand()%state->rows;
    int x=rand()%state->cols;
    if(state->board[y][x]==0 && !(x == state->pacman_x && y == state->pacman_y)){
      state->board[y][x]=1;
      pawn_count--;
    }
  }
}

// Déplacez Pac-Man dans la direction indiquée et vérifiez s'il a mangé des haricots
void move_pacman(GameState *state,Direction dir){
  int new_x=state->pacman_x;
  int new_y=state->pacman_y;

  switch(dir){
  case UP:
    if(state->pacman_y>0){
      new_y--;
      state->dir=UP;
    }
    break;
  case DOWN:
    if(state->pacman_y<state->rows-1){
      new_y++;
      state->dir=DOWN;
    }
    break;
  case LEFT:
    if(state->pacman_x>0){
      new_x--;
      state->dir=LEFT;
    }
    break;
  case RIGHT:
    if(state->pacman_x<state->cols-1){
      new_x++;
      state->dir=RIGHT;
    }
    break;
  }

  if(new_x!=state->pacman_x || new_y!=state->pacman_y){
    state->pacman_x=new_x;
    state->pacman_y=new_y;
    state->move_count++;

    if(state->board[new_y][new_x]==1){
      state->board[new_y][new_x]=0;
      state->eaten_count++;
    }
  }
}

// Choisissez au hasard une direction pour déplacer Pac-Man
void move_pacman_randomly(GameState *state){
  Direction dir=(Direction)(rand()%4);
  move_pacman(state,dir);
}

// Déplacez Pac-Man en utilisant un motif en zigzag
void move_pacman_zigzag(GameState *state) {
  static int zigzag_direction = RIGHT;
  int old_x = state->pacman_x;
  int old_y = state->pacman_y;

  // Vérifiez si c'est la dernière ligne
  if (state->pacman_y == state->rows-1) {
    // S'il s'agit de la dernière grille dans le coin inférieur droit et se déplace vers la droite
    if (state->pacman_x == state->cols-1 && zigzag_direction == RIGHT && has_pawn_to_eat(state)) {
      // Retour à la première grille dans le coin supérieur gauche
      state->pacman_x = 0;
      state->pacman_y = 0;
      state->move_count++;
      
      // Mangez le haricot dans le coin supérieur gauche (s'il y en a un)
      if (state->board[0][0] == 1) {
        state->board[0][0] = 0;
        state->eaten_count++;
      }      
      // Continuez à vous déplacer vers la droite
      zigzag_direction = RIGHT;
      return;
    }
    // S'il s'agit de la dernière grille dans le coin inférieur gauche et se déplace vers la gauche
    else if (state->pacman_x == 0 && zigzag_direction == LEFT && has_pawn_to_eat(state)) {
      // Retour au coin supérieur droit
      state->pacman_x = state->cols-1;
      state->pacman_y = 0;
      state->move_count++;
      
      // Mangez le haricot dans le coin supérieur droit (s'il y en a un)
      if (state->board[0][state->cols-1] == 1) {
        state->board[0][state->cols-1] = 0;
        state->eaten_count++;
      }      
      // Continuer à se déplacer vers la gauche
      zigzag_direction = LEFT;
      return;
    }
  }
  
  if (zigzag_direction == RIGHT) {
    move_pacman(state, RIGHT);
    
    if (state->pacman_x == old_x) {
      move_pacman(state, DOWN);
      
      if (state->pacman_y != old_y) {
        zigzag_direction = LEFT;
      } else {
        zigzag_direction = LEFT;
        move_pacman(state, LEFT);
      }
    }
  } else {
    move_pacman(state, LEFT);
    
    if (state->pacman_x == old_x) {
      move_pacman(state, DOWN);
      
      if (state->pacman_y != old_y) {
        zigzag_direction = RIGHT;
      } else {
        zigzag_direction = RIGHT;
        move_pacman(state, RIGHT);
      }
    }
  }
}

// Recherche en profondeur pour trouver le bean le plus proche, en parcourant récursivement les chemins possibles
static void dfs(GameState *state, int x, int y, int **visited, int *found, int *next_x, int *next_y){
  // Vérifier les conditions aux limites et l'état visité
  if(y < 0 || y >= state->rows || x < 0 || x >= state->cols || visited[y][x]){
    return;
  }
  visited[y][x]=1;

  if(state->board[y][x]==1){
    *found=1;
    *next_x=x;
    *next_y=y;
    return;
  }

  // Utilisez une plage de recherche plus large pour prioriser les directions qui n'ont pas été visitées auparavant
  static Direction last_directions[4] = {UP, DOWN, LEFT, RIGHT};
  
  // Mélangez aléatoirement l'ordre de direction à chaque fois que vous effectuez une recherche pour augmenter la diversité de la recherche
  for(int i = 0; i < 4; i++){
    int j = rand() % 4;
    Direction temp = last_directions[i];
    last_directions[i] = last_directions[j];
    last_directions[j] = temp;
  }
  
  // Explorez quatre directions dans un ordre aléatoire
  for(int i = 0; i < 4; i++){
    switch(last_directions[i]){
      case UP:
        dfs(state, x, y-1, visited, found, next_x, next_y);
        break;
      case DOWN:
        dfs(state, x, y+1, visited, found, next_x, next_y);
        break;
      case LEFT:
        dfs(state, x-1, y, visited, found, next_x, next_y);
        break;
      case RIGHT:
        dfs(state, x+1, y, visited, found, next_x, next_y);
        break;
    }
    if(*found) return;
  }
}

// Utilisez l'algorithme DFS pour trouver et déplacer intelligemment Pac-Man vers la position de haricot la plus proche
void move_pacman_dfs(GameState *state){
  
  if(state == NULL || state->rows <= 0 || state->cols <= 0) {
    return;
  }
  
  // Ajouter une détection d'errance
  #define HISTORY_SIZE 20
  static int history_x[HISTORY_SIZE] = {0};
  static int history_y[HISTORY_SIZE] = {0};
  static int history_index = 0;
  static int oscillation_count = 0;
  static int forced_direction = -1;
  static int forced_steps_remaining = 0;
  
  // Vérifier si le survol (déplacement d'avant en arrière entre deux positions)
  int is_oscillating = 0;
  if(history_index >= 4) {
    // Vérifier le modèle A-B-A-B
    int pos1 = (history_index - 1) & (HISTORY_SIZE - 1);
    int pos2 = (history_index - 2) & (HISTORY_SIZE - 1);
    int pos3 = (history_index - 3) & (HISTORY_SIZE - 1);
    int pos4 = (history_index - 4) & (HISTORY_SIZE - 1);
    
    if((history_x[pos1] == history_x[pos3] && history_y[pos1] == history_y[pos3]) &&
       (history_x[pos2] == history_x[pos4] && history_y[pos2] == history_y[pos4]) &&
       (history_x[pos1] != history_x[pos2] || history_y[pos1] != history_y[pos2])) {
      is_oscillating = 1;
      oscillation_count++;
    }
  }
  
  // Si une errance est détectée et que le nombre de fois dépasse le seuil, forcer une direction fixe pour se déplacer
  if(is_oscillating && oscillation_count > 2 && forced_steps_remaining <= 0) {
    oscillation_count = 0;
    // Sélectionnez aléatoirement une direction et forcez quelques pas dans cette direction
    forced_direction = rand() % 4;
    forced_steps_remaining = 3 + (rand() % 3); 
  }
  
  if(forced_steps_remaining > 0) {
    Direction dir = (Direction)forced_direction;
    int old_x = state->pacman_x;
    int old_y = state->pacman_y;
    
    move_pacman(state, dir);
    
    // Si le déplacement est réussi (position modifiée), réduire le nombre de pas forcés
    if(old_x != state->pacman_x || old_y != state->pacman_y) {
      forced_steps_remaining--;
    } else {
      // Si bloqué par un mur, changez de direction
      forced_direction = (forced_direction + 1) % 4;
    }
    
    // Enregistrer l'historique des mouvements
    history_x[history_index] = state->pacman_x;
    history_y[history_index] = state->pacman_y;
    history_index = (history_index + 1) % HISTORY_SIZE;
    
    return;
  }
  
  int **visited = (int **)calloc(state->rows, sizeof(int *));
  if(visited == NULL) {
    return;
  }
  
  for(int i = 0; i < state->rows; i++){
    visited[i] = (int *)calloc(state->cols, sizeof(int));
    if(visited[i] == NULL) {
      for(int j = 0; j < i; j++){
        free(visited[j]);
      }
      free(visited);
      return;
    }
  }

  int found = 0;
  int next_x = 0, next_y = 0;
  
  // Assurez-vous que les coordonnées de Pacman sont dans la plage valide
  if(state->pacman_y >= 0 && state->pacman_y < state->rows && 
     state->pacman_x >= 0 && state->pacman_x < state->cols) {
    dfs(state, state->pacman_x, state->pacman_y, visited, &found, &next_x, &next_y);
  }

  for(int i = 0; i < state->rows; i++){
    free(visited[i]);
  }
  free(visited);

  // Assurez-vous qu'il bouge
  if(found) {
    // Trouvez la pièce d'échecs et déplacez-vous dans la direction de la pièce d'échecs
    Direction move_dir;
    
    if(next_x > state->pacman_x) {
      move_dir = RIGHT;
    }
    else if(next_x < state->pacman_x) {
      move_dir = LEFT;
    }
    else if(next_y > state->pacman_y) {
      move_dir = DOWN;
    }
    else {
      move_dir = UP;
    }
    
    int old_x = state->pacman_x;
    int old_y = state->pacman_y;
    move_pacman(state, move_dir);
    
    // Enregistrer l'historique uniquement s'il a réellement bougé
    if(old_x != state->pacman_x || old_y != state->pacman_y) {
      history_x[history_index] = state->pacman_x;
      history_y[history_index] = state->pacman_y;
      history_index = (history_index + 1) % HISTORY_SIZE;
    }
  } else {
    // Construire une carte de fréquence d'accès
    int **frequency = (int **)calloc(state->rows, sizeof(int *));
    for(int i = 0; i < state->rows; i++){
      frequency[i] = (int *)calloc(state->cols, sizeof(int));
    }
    
    // Remplir la carte de fréquence en fonction des enregistrements historiques
    for(int i = 0; i < HISTORY_SIZE; i++){
      int idx = (history_index - i - 1 + HISTORY_SIZE) % HISTORY_SIZE;
      if(history_x[idx] >= 0 && history_x[idx] < state->cols && 
         history_y[idx] >= 0 && history_y[idx] < state->rows) {
        // L'accès le plus récent a un poids plus élevé
        frequency[history_y[idx]][history_x[idx]] += (HISTORY_SIZE - i);
      }
    }
    
    // Créer une liste de directions de mouvement possibles
    int num_dirs = 0;
    Direction possible_dirs[4];
    int weights[4] = {0};
    
    // Vérifiez les quatre directions et calculez les poids
    struct {
      Direction dir;
      int dx, dy;
    } directions[] = {
      {UP, 0, -1},
      {DOWN, 0, 1},
      {LEFT, -1, 0},
      {RIGHT, 1, 0}
    };
    
    for(int i = 0; i < 4; i++) {
      int new_x = state->pacman_x + directions[i].dx;
      int new_y = state->pacman_y + directions[i].dy;
      
      if(new_x >= 0 && new_x < state->cols && new_y >= 0 && new_y < state->rows) {
        possible_dirs[num_dirs] = directions[i].dir;
        
        // Calculer les poids, plus la fréquence est élevée, plus le poids est faible
        int freq = frequency[new_y][new_x];
        
        // Évitez de revenir spécifiquement à la position précédente
        int last_pos = (history_index - 1 + HISTORY_SIZE) % HISTORY_SIZE;
        if(new_x == history_x[last_pos] && new_y == history_y[last_pos]) {
          weights[num_dirs] = 1; // Poids très faible
        } else {
          weights[num_dirs] = 1000 - freq * 50;
          if(weights[num_dirs] < 10) weights[num_dirs] = 10;
        }
        
        num_dirs++;
      }
    }
    
    // Libérer la mémoire de la carte de fréquence
    for(int i = 0; i < state->rows; i++){
      free(frequency[i]);
    }
    free(frequency);
    
    if(num_dirs > 0) {
      // Sélectionnez la direction en fonction du poids
      int total_weight = 0;
      for(int i = 0; i < num_dirs; i++) {
        total_weight += weights[i];
      }
      
      int random_val = rand() % total_weight;
      int selected_dir = 0;
      int weight_sum = 0;
      
      for(int i = 0; i < num_dirs; i++) {
        weight_sum += weights[i];
        if(random_val < weight_sum) {
          selected_dir = i;
          break;
        }
      }
      
      int old_x = state->pacman_x;
      int old_y = state->pacman_y;
      move_pacman(state, possible_dirs[selected_dir]);
      
      // Enregistrer l'historique uniquement s'il a réellement bougé
      if(old_x != state->pacman_x || old_y != state->pacman_y) {
        history_x[history_index] = state->pacman_x;
        history_y[history_index] = state->pacman_y;
        history_index = (history_index + 1) % HISTORY_SIZE;
      }
    }
  }
}

// Vérifiez s'il reste des haricots sur le plateau de jeu
int has_pawn_to_eat(GameState *state){
  for(int i=0;i<state->rows;i++){
    for(int j=0;j<state->cols;j++){
      if(state->board[i][j]==1){
	return 1;
      }
    }
  }
  return 0;
}

// Obtenir le nombre actuel de mouvements
int get_move_count(GameState *state){
  return state->move_count;
}

// Obtenir le nombre de haricots mangés
int get_eaten_count(GameState *state){
  return state->eaten_count;
}

