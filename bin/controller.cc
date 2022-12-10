#include "controller.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdexcept>
#include <iostream>

#include "Timer.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "Action.h"

__BEGIN_API

Controller::Controller(bool *finish, Ship * shipVec)
{
    ship = shipVec;
    _finish = finish;
    acao = act::action::NO_ACTION;
}

void Controller::input() {
    while (!*_finish)
    {
        if (al_key_down(&kb, ALLEGRO_KEY_UP)) {
            ship->putY(-280);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_RIGHT)) {
            ship->putX(280);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_DOWN)) {
            ship->putY(280);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_LEFT)) {
            ship->putX(-280);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_1)) {
            acao = act::action::FIRE_PRIMARY;
        }
        if (al_key_down(&kb, ALLEGRO_KEY_SPACE)) {
            acao = act::action::FIRE_SECONDARY;
        }
        if (al_key_down(&kb, ALLEGRO_KEY_ESCAPE)) {
            std::cout << "sair\n";
            acao = act::action::QUIT_GAME;
        }
        Thread::yield();
    }

    Thread::running()->thread_exit(0);

}



__END_API