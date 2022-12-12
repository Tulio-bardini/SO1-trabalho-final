#ifndef ship_H
#define ship_H

#include <allegro5/allegro.h>
#include <memory>
#include <string>

#include "Sprite.h"
#include "Vector.h"
#include "Action.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "Laser.h"

__BEGIN_API

#define SHIP_SIZE 16

class Ship {

public:
    Ship(bool *finish, std::list<Laser> *lasers, float *dt);
    ~Ship();
    void run(); 
    void putX(int x);
    void putY(int y);
    void draw();
    void drawLife();
    void fire();
    void hit(int damage);

    std::list<Laser> * lasersShip;
    std::shared_ptr<Sprite> shipSprite;
    Point centre;
    int row; /**<row of animation to be played */
    int col; /**< column of animation to be played */
    int size = SHIP_SIZE;
    bool dead = false;
    int life = 3;
    int lifeRestart = 2;
    Vector speed;

private:
    void selectShipAnimation();
    void checkBoundary();

    bool *_finish;
    float *_dt;

};



__END_API


#endif