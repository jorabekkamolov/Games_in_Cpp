#include "game.h"

Game::Game(QWidget *parent) : QWidget{parent}, SnakeGame(5, 5) {
  timerId = startTimer(gameinfo.speed);
}

void Game::resetGame() {
  snake.clear();
  gameinfo.game_over = false;
  gameinfo.score = 0;
  gameinfo.level = 0;
  gameinfo.speed = 350;
  gameinfo.pause = 0;
  for (int i = 0; i < HEIGHT; i++) {
    delete[] gameinfo.field[i];
  }
  delete[] gameinfo.field;
  gameinfo.field = new int *[HEIGHT];
  for (int i = 0; i < HEIGHT; i++) {
    gameinfo.field[i] = new int[WIDTH];
    for (int j = 0; j < WIDTH; j++) {
      gameinfo.field[i][j] = 0;
    }
  }
  snake.push_back({5, 5});
  random_apple();
  place_snake_in_field();
  lastAction = Right;
  keyPressed = false;
  killTimer(timerId);
  timerId = startTimer(gameinfo.speed);
  QWidget::update();
}

void Game::gameover() {
  int ret = QMessageBox::question(
      this, "GAME OVER",
      QString("Level: %1  Score: %2\nDo you want to exit or start a new game?")
          .arg(gameinfo.level)
          .arg(gameinfo.score),
      QMessageBox::Yes | QMessageBox::No);
  if (ret == QMessageBox::Yes) {
    killTimer(timerId);
    qApp->quit();
  } else {
    resetGame();
  }
}

void Game::winner() {
  killTimer(timerId);
  int ret =
      QMessageBox::question(this, "WINNER", "Do you want to exit or continue?",
                            QMessageBox::Yes | QMessageBox::No);
  if (ret == QMessageBox::Yes) {
    killTimer(timerId);
    qApp->quit();
  } else {
    timerId = startTimer(gameinfo.speed);
  }
}

void Game::pause() {
  killTimer(timerId);
  int ret = QMessageBox::question(
      this, "PAUSE",
      QString("Level: %1  Score: %2\nDo you want to exit or continue?")
          .arg(gameinfo.level)
          .arg(gameinfo.score),
      QMessageBox::Yes | QMessageBox::No);
  if (ret == QMessageBox::Yes) {
    killTimer(timerId);
    qApp->quit();
  } else {
    gameinfo.pause = 0;
    timerId = startTimer(gameinfo.speed);
  }
}

void Game::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.fillRect(rect(), Qt::darkGreen);

  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
        painter.setBrush(Qt::gray);
        painter.drawRect(j * 20, i * 20, 20, 20);
      } else if (gameinfo.field[i][j] == 1) {
        if (snake.front().first == i && snake.front().second == j) {
          painter.setBrush(Qt::white);
        } else {
          painter.setBrush(Qt::green);
        }
        painter.drawEllipse(j * 20, i * 20, 20, 20);
      } else if (gameinfo.field[i][j] == 2) {
        painter.setBrush(Qt::red);
        painter.drawEllipse(j * 20, i * 20, 20, 20);
      }
    }
  }
  painter.setPen(Qt::black);
  painter.setFont(QFont("Arial", 14, QFont::Bold));

  int rightMargin = WIDTH * 20 + 20;

  painter.drawText(rightMargin, 30, QString("Score: %1").arg(gameinfo.score));
  painter.drawText(rightMargin, 50, QString("Level: %1").arg(gameinfo.level));
  painter.drawText(rightMargin, 70, QString("Speed: %1").arg(gameinfo.speed));
  painter.drawText(rightMargin, 90,
                   QString("High Score: %1").arg(gameinfo.high_score));
}

void Game::keyPressEvent(QKeyEvent *e) {
  keyPressed = true;
  killTimer(timerId);
  timerId = startTimer(gameinfo.speed);
  int key = e->key();
  switch (key) {
    case Qt::Key_Left:
      change_direction(Left);
      lastAction = Left;
      break;
    case Qt::Key_Right:
      change_direction(Right);
      lastAction = Right;
      break;
    case Qt::Key_Up:
      change_direction(Up);
      lastAction = Up;
      break;
    case Qt::Key_Down:
      change_direction(Down);
      lastAction = Down;
      break;
    case Qt::Key_P:
      gameinfo.pause = 1;
      break;
  }

  check();
  if (gameinfo.game_over) {
    QWidget::update();
  } else {
    SnakeGame::update();
    QWidget::update();
  }
}

void Game::timerEvent(QTimerEvent *) {
  if (!keyPressed) {
    change_direction(lastAction);
  }
  check();

  if (gameinfo.game_over) {
    gameover();
  } else if (gameinfo.score == 200) {
    winner();
  } else if (gameinfo.pause) {
    pause();
  } else {
    SnakeGame::update();
    QWidget::update();
    keyPressed = false;
  }
}
