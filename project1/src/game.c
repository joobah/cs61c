#include "game.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_t *game, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_t *game, unsigned int snum);
static char next_square(game_t *game, unsigned int snum);
static void update_tail(game_t *game, unsigned int snum);
static void update_head(game_t *game, unsigned int snum);

/* Constants */
const char* SNAKE_HEAD_CHARS = "WASDx";
const char* SNAKE_BODY_CHARS = "^<v>";
const char* SNAKE_TAIL_CHARS = "wasd";
const size_t READ_BUFFER = 10;

/* Task 1 */
game_t *create_default_game() {
  // Create the game struct on the heap
  game_t *game = malloc(sizeof(game_t));

  // Create the snake struct
  game->num_snakes = 1;
  snake_t *snake = malloc(sizeof(snake_t) * game->num_snakes);
  snake->head_row = 2;
  snake->head_col = 4;
  snake->tail_row = 2;
  snake->tail_col = 2;
  snake->live = true;
  game->snakes = snake;

  // Create the board
  game->num_rows = 18;
  char **board = malloc(sizeof(char*) * game->num_rows);
  for(int i = 0; i < game->num_rows; i++) {
    board[i] = malloc(sizeof(char) * 22); // 20 columns +1 (newline) +1 (null terminator)
    strcpy(board[i], "#                  #\n");
  }

  // Fix the other rows
  strcpy(board[0], "####################\n");
  strcpy(board[2], "# d>D    *         #\n");
  strcpy(board[game->num_rows-1], "####################\n");
  game->board = board;
  
  return game;
}

/* Task 2 */
void free_game(game_t *game) {
  free(game->snakes);
  for(int i = 0; i < game->num_rows; i++) {
    free(game->board[i]);
  }
  free(game->board);
  free(game);
}

/* Task 3 */
void print_board(game_t *game, FILE *fp) {
  for(int i = 0; i < game->num_rows; i++) {
    fprintf(fp, game->board[i]);
  }
}

/*
  Saves the current game into filename. Does not modify the game object.
  (already implemented for you).
*/
void save_board(game_t *game, char *filename) {
  FILE *f = fopen(filename, "wb");
  print_board(game, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_t *game, unsigned int row, unsigned int col) { return game->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_t *game, unsigned int row, unsigned int col, char ch) {
  game->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  for(int i = 0; i < strlen(SNAKE_TAIL_CHARS); i++) {
    if(c == SNAKE_TAIL_CHARS[i]) {
      return true;
    }
  }
  return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  for(unsigned int i = 0; i < strlen(SNAKE_HEAD_CHARS); i++) {
    if(c == SNAKE_HEAD_CHARS[i]) {
      return true;
    }
  }
  return false;
}

/*
  Returns true if c is part of the snake's body.
*/
static bool is_body(char c) {
  for(int i = 0; i < strlen(SNAKE_BODY_CHARS); i++) {
    if(c == SNAKE_BODY_CHARS[i]) {
      return true;
    }
  }
  return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  return is_head(c) || is_tail(c) || is_body(c);
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  for(int i = 0; i < strlen(SNAKE_BODY_CHARS); i++) {
    if(c == SNAKE_BODY_CHARS[i]) {
      return SNAKE_TAIL_CHARS[i];
    }
  }

  return c;
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  for(int i = 0; i < strlen(SNAKE_HEAD_CHARS); i++) {
    if(c == SNAKE_HEAD_CHARS[i]) {
      return SNAKE_BODY_CHARS[i];
    }
  }

  return c;
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  if(c == 'v' || c == 's' || c == 'S') {
    return cur_row + 1;
  }
  
  if(c == '^' || c == 'w' || c == 'W') {
    return cur_row - 1;
  }

  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  if(c == '>' || c == 'd' || c == 'D') {
    return cur_col + 1;
  }
  
  if(c == '<' || c == 'a' || c == 'A') {
    return cur_col - 1;
  }

  return cur_col;
}

/*
  Task 4.2

  Helper function for update_game. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_t *game, unsigned int snum) {
  snake_t *snake = (game->snakes + snum);
  char snake_head = get_board_at(game, snake->head_row, snake->head_col);

  return get_board_at(game, get_next_row(snake->head_row, snake_head), get_next_col(snake->head_col, snake_head));
}

/*
  Task 4.3

  Helper function for update_game. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_t *game, unsigned int snum) {
  snake_t *snake = (game->snakes + snum);
  char snake_head = get_board_at(game, snake->head_row, snake->head_col);
  
  unsigned int next_row = get_next_row(snake->head_row, snake_head);
  unsigned int next_col = get_next_col(snake->head_col, snake_head);

  // Replace current head with body
  char new_snake_body = head_to_body(snake_head);
  set_board_at(game, snake->head_row, snake->head_col, new_snake_body);

  // Set new position on board with snake head
  set_board_at(game, next_row, next_col, snake_head);

  // Update snake struct
  snake->head_row = next_row;
  snake->head_col = next_col;
}

/*
  Task 4.4

  Helper function for update_game. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_t *game, unsigned int snum) {
  snake_t *snake = (game->snakes + snum);
  char snake_tail = get_board_at(game, snake->tail_row, snake->tail_col);
  
  unsigned int next_row = get_next_row(snake->tail_row, snake_tail);
  unsigned int next_col = get_next_col(snake->tail_col, snake_tail);

  // Blank out current tail
  set_board_at(game, snake->tail_row, snake->tail_col, ' ');

  // Replace snake body with snake tail
  char snake_body = get_board_at(game, next_row, next_col);
  char new_snake_tail = body_to_tail(snake_body);
  set_board_at(game, next_row, next_col, new_snake_tail);

  // Update snake struct
  snake->tail_row = next_row;
  snake->tail_col = next_col;
}

/* Task 4.5 */
void update_game(game_t *game, int (*add_food)(game_t *game)) {
  for(unsigned int snum = 0; snum < game->num_snakes; snum++) {
    snake_t *snake = game->snakes + snum;

    // Don't move dead snake
    if(!snake->live) {
      continue;
    }

    char next_square_for_snake = next_square(game, snum);

    // snake dies if the next square is a wall or snake
    if(next_square_for_snake == '#' || is_snake(next_square_for_snake)) {
      set_board_at(game, snake->head_row, snake->head_col, 'x');
      snake->live = false;
    } else if (next_square_for_snake == '*') { // If food, only update head and add new food
      update_head(game, snum);
      add_food(game);
    } else {
      update_head(game, snum);
      update_tail(game, snum);
    }
  }
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  char* line = NULL;
  char temp[READ_BUFFER + 1];
  unsigned int loop_index = 0;

  while(fgets(temp, (int)READ_BUFFER + 1, fp) != NULL) {
    // size 11, 21, 31, etc.
    line = realloc(line, sizeof(char) * (READ_BUFFER * (loop_index + 1) + 1));

    // copy temp to 0, 10, 20, etc.
    strcpy(line + loop_index * READ_BUFFER, temp);

    // Exit if we read the newline
    if(strchr(temp, '\n') != NULL) {
      break;
    }
    
    loop_index++;
  }
  return line;
}

/* Task 5.2 */
game_t *load_board(FILE *fp) {
  game_t *game = malloc(sizeof(game_t));

  unsigned int rows = 0;
  char **board = NULL;

  while (true) {
    char *line = read_line(fp);
    if(line == NULL) {
      break;
    };
    rows++;
    board = realloc(board, sizeof(char*) * rows);
    board[rows - 1] = line;
  }

  game->board = board;
  game->num_rows = rows;
  game->snakes = NULL;
  
  return game;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_t *game, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_t *initialize_snakes(game_t *game) {
  // TODO: Implement this function.
  return NULL;
}
