#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#define WIDTH 10
#define HEIGHT 20

#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

typedef enum {
  Left,
  Right,
  Up,
  Down,
} UserAction_t;

typedef struct {
  int **field;
  bool game_over;
  int score;
  int level;
  int speed;
  int high_score;
  int pause;
} GameInfo_t;

class SnakeGame {
 public:
  std::vector<std::pair<int, int>> snake;
  GameInfo_t gameinfo;
  UserAction_t current_action = Right;
  int r_y = 0;
  int r_x = 0;
  SnakeGame(int x_snake, int y_snake);
  ~SnakeGame();
  void random_apple();
  bool check_random_apple();
  void place_snake_in_field();
  void change_direction(UserAction_t action);
  void move_snake();
  void update();
  void check();
  bool check_snake();
};

#endif  // SNAKEGAME_H
