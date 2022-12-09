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

Controller::Controller(Ship * shipVec)
{
    ship = shipVec;
}

void Controller::input() {
    while (1==1)
    {
        if (al_key_down(&kb, ALLEGRO_KEY_UP)) {
            ship->putY(-250);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_RIGHT)) {
            ship->putX(250);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_DOWN)) {
            ship->putY(250);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_LEFT)) {
            ship->putX(-250);
        }
        if (al_key_down(&kb, ALLEGRO_KEY_1)) {
            std::cout << "missel\n";
            act::action::FIRE_PRIMARY;
        }
        if (al_key_down(&kb, ALLEGRO_KEY_SPACE)) {
            std::cout << "tiro normal\n";
            act::action::FIRE_SECONDARY;
        }
        if (al_key_down(&kb, ALLEGRO_KEY_ESCAPE)) {
            std::cout << "sair\n";
            act::action::QUIT_GAME;
        }
        Thread::yield();
    }
    
}



__END_API