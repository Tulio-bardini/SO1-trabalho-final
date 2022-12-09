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

__BEGIN_API

class Ship {

public:
    Ship();
    void update();
    void putX(int x);
    void putY(int y);

    Point centre;
    int row; /**<row of animation to be played */
    int col; /**< column of animation to be played */
private:
    void selectShipAnimation();
    void checkBoundary();
    Vector speed;

    // general game variables

};



__END_API


#endif