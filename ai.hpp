#ifndef GWANKEI_AI_HPP
#define GWANKEI_AI_HPP


#include <QObject>
#include "game.hpp"


using namespace GwanKei;


class AI : public QObject {
  Q_OBJECT
private:
  Player player;
  bool initialized = false;
  bool ended = false; // game over -> true
public:
  AI();
  Player get_player() const;
  void set_player(Player player);
  bool is_initialized() const;
  bool is_ended() const;
  virtual Layout get_layout() = 0;
signals:
  void move(Cell from, Cell to);
public slots:
  void game_over();
  virtual void status_changed(Game game, Player current_player) = 0;
};


class Brainless : public AI {
  Q_OBJECT
public:
  Brainless();
  Layout get_layout();
  static Brainless* Create();
public slots:
  void status_changed(Game game, Player current_player);
};


struct CellPair {
  Cell first;
  Cell second;
};


class LowIQ : public AI {
  Q_OBJECT
private:
  double aggressive;
  int least[105] = {0}; // 記錄最小猜測，下標為 Element::id
  int num_of_kill[105] = {0}; // 記錄吃子個數，下標同
  bool is_not_mine[105] = {0}; // 是否確信【不是】地雷，注意不要在後兩排外使用
  Game last_game;
  bool is_proper_layout(const Layout& layout) const;
public:
  LowIQ(double aggressive = 0.5);
  Layout get_layout();
  static LowIQ* Rand();
public slots:
  void status_changed(Game game, Player current_player);
};


#endif //GWANKEI_AI_HPP
