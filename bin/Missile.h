/**
 * @file Missile.h
 * @brief
 *
 * @author
 * @bug
 */
#ifndef MISSILE_H
#define MISSILE_H

#include <allegro5/allegro.h>
#include <memory>
#include <vector>

#include "Point.h"
#include "Vector.h"
#include "Sprite.h"
#include "Action.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "Laser.h"

__BEGIN_API

class Missile
{
public:
  Missile(Point p, Vector s, double angle, int owner, float * dt);
  ~Missile();

  void load_sprites();
  void draw();
  void deathAnim();
  void update();

  bool dead = false;
  Point centre;
  Vector speed;
  bool live = true;
  int classOwner;

  static void runMissile(std::list<std::shared_ptr<Missile>> *missiles, bool *finish);

private:
  std::shared_ptr<Sprite> missileSprite1;
  std::shared_ptr<Sprite> missileSprite2;
  std::shared_ptr<Sprite> missileSprite3;
  std::shared_ptr<Sprite> missileSprite4;
  std::shared_ptr<Sprite> missileSprite5;
  std::shared_ptr<Sprite> missileSprite6;
  std::shared_ptr<Sprite> missileSprite7;
  std::shared_ptr<Sprite> deathSprite;

  bool dAnim_complete;
  int mAnim;
  int dAnim;
  bool in_bound();
  double _angle;
  float *_dt;
};

__END_API

#endif
