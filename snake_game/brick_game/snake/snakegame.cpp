#include "snakegame.h"

SnakeGame::SnakeGame(int x_snake, int y_snake) {
  std::ifstream file("../../brick_game/snake/db.txt");
  snake.push_back({y_snake, x_snake});
  gameinfo.score = 0;
  gameinfo.level = 0;
  gameinfo.speed = 350;
  gameinfo.high_score = 0;
  gameinfo.pause = 0;
  int num;
  while (file >> num) {
    std::cout << num;
    gameinfo.high_score = gameinfo.high_score * 10 + num;
  }
  file.close();
  gameinfo.game_over = false;
  gameinfo.field = new int*[HEIGHT];
  for (int i = 0; i < HEIGHT; i++) {
    gameinfo.field[i] = new int[WIDTH];
    for (int j = 0; j < WIDTH; j++) {
      gameinfo.field[i][j] = 0;
    }
  }
  random_apple();
  place_snake_in_field();
}

SnakeGame::~SnakeGame() {
  for (int i = 0; i < HEIGHT; i++) {
    delete[] gameinfo.field[i];
  }
  delete[] gameinfo.field;
}

void SnakeGame::random_apple() {
  srand(time(0));
  r_y = rand() % 20;
  r_x = rand() % 10;
  while (check_random_apple()) {
    r_y = rand() % 20;
    r_x = rand() % 10;
  }
}

bool SnakeGame::check_random_apple() {
  bool temp = false;
  for (size_t i = 0; i < snake.size(); i++) {
    if (r_y == snake[i].first && r_x == snake[i].second) {
      temp = true;
    }
  }
  if (r_y == 0 || r_y == HEIGHT - 1 || r_x == 0 || r_x == WIDTH - 1) {
    temp = 1;
  }
  return temp;
}

void SnakeGame::place_snake_in_field() {
  for (size_t i = 0; i < snake.size(); i++) {
    gameinfo.field[snake[i].first][snake[i].second] = 1;
  }
  gameinfo.field[r_y][r_x] = 2;
}

void SnakeGame::change_direction(UserAction_t action) {
  if (snake.size() > 1) {
    if (current_action == Left && action != Right) {
      current_action = action;
    } else if (current_action == Right && action != Left) {
      current_action = action;
    } else if (current_action == Up && action != Down) {
      current_action = action;
    } else if (current_action == Down && action != Up) {
      current_action = action;
    }
  } else {
    current_action = action;
  }
}

void SnakeGame::update() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      gameinfo.field[i][j] = 0;
    }
  }
  move_snake();
  place_snake_in_field();
}

void SnakeGame::move_snake() {
  auto head = snake.front();
  switch (current_action) {
    case Up:
      head.first--;
      break;
    case Down:
      head.first++;
      break;
    case Left:
      head.second--;
      break;
    case Right:
      head.second++;
      break;
  }
  snake.insert(snake.begin(), head);
  if (head.first == r_y && head.second == r_x) {
    random_apple();
    ++gameinfo.score;
    if (gameinfo.score % 5 == 0 && gameinfo.level != 10) {
      ++gameinfo.level;
    }
    if (gameinfo.score % 5 == 0) {
      gameinfo.speed -= 10;
    }
    if (gameinfo.score > gameinfo.high_score) {
      std::fstream file("../../brick_game/snake/db.txt",
                        std::ios::out | std::ios::trunc);
      file << gameinfo.score << std::endl;
      file.close();
    }
  } else {
    snake.pop_back();
  }
}

void SnakeGame::check() {
  if ((current_action == Left && snake.front().second == 1) || check_snake()) {
    gameinfo.game_over = true;
  } else if ((current_action == Right && snake.front().second == WIDTH - 2) ||
             check_snake()) {
    gameinfo.game_over = true;
  } else if ((current_action == Up && snake.front().first == 1) ||
             check_snake()) {
    gameinfo.game_over = true;
  } else if ((current_action == Down && snake.front().first == HEIGHT - 2) ||
             check_snake()) {
    gameinfo.game_over = true;
  }
}

bool SnakeGame::check_snake() {
  bool game_over = false;
  if (current_action == Left &&
      (gameinfo.field[snake.front().first][(snake.front().second) - 1]) == 1) {
    game_over = true;
  } else if (current_action == Right &&
             (gameinfo.field[snake.front().first][(snake.front().second) + 1] ==
              1)) {
    game_over = true;
  } else if (current_action == Up &&
             (gameinfo.field[(snake.front().first) - 1][snake.front().second] ==
              1)) {
    game_over = true;
  } else if (current_action == Down &&
             (gameinfo.field[(snake.front().first) + 1][snake.front().second] ==
              1)) {
    game_over = true;
  }
  return game_over;
}
