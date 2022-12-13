#ifndef controller_H
#define controller_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <string>

#include "Timer.h"
#include "ship.h"
#include "Sprite.h"
#include "Vector.h"
#include "Action.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"

__BEGIN_API

class Controller
{

public:
    Controller(bool *finish, Ship *shipVec);

    void input();
    act::action acao;
    ALLEGRO_KEYBOARD_STATE kb;

private:
    Ship *ship;
    bool *_finish;
    // general game variables
};

__END_API

#endif