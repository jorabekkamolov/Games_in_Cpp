#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/snake/snakegame.h"

class Game : public QWidget, public SnakeGame {
 public:
  explicit Game(QWidget *parent = nullptr);
  int timerId = 0;
  bool keyPressed = false;
  UserAction_t lastAction = Right;

 protected:
  void timerEvent(QTimerEvent *) override;
  void paintEvent(QPaintEvent *) override;
  void keyPressEvent(QKeyEvent *) override;

 public:
  void resetGame();
  void gameover();
  void winner();
  void pause();
};
