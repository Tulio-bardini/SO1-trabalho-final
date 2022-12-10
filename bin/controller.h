#ifndef controller_H
#define controller_H

#include <allegro5/allegro.h>
#include <memory>
#include <string>

#include "ship.h"
#include "Sprite.h"
#include "Vector.h"
#include "Action.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"

__BEGIN_API

class Controller {

public:
    Controller(bool *finish ,Ship *shipVec);

    void input();
    act::action acao;
    ALLEGRO_KEYBOARD_STATE kb;
private:
    Ship * ship;
    bool * _finish;
    // general game variables
};

__END_API

#endif