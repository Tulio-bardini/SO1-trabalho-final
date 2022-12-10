/**
 * @file EnemyPurple.cc
 * @brief Implementation of EnemyPurple class- draws, updates and destroys enemies
 *
 * @author
 * @bug
 **/
#include "enemyPurple.h"

#include "Point.h"
#include "Vector.h"
#include "Timer.h"
#include "Sprite.h"

const int EnemyPurple_SIZE = 20;

EnemyPurple::EnemyPurple(Point cen, ALLEGRO_COLOR col, Vector spd, float *dt)
{
    speed = spd;
    color = col;
    centre = cen;
    _dt = dt;

    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(path, "resources");
    al_change_directory(al_path_cstr(path, '/'));

    enemySprite = std::make_shared<Sprite> ("EnemyBasic.png"); //espaçonave do enimigo
    deathSprite = std::make_shared<Sprite> ("explode.png"); //animação explosão
    load_assets();

}
						
// creates and starts the fireDelay timer for the EnemyPurple
void EnemyPurple::load_assets() {
   fireDelay = std::make_shared<Timer> (60);   
   fireDelay->create();
   fireDelay->startTimer();
}
	
EnemyPurple::~EnemyPurple() {
   fireDelay.reset();
}

// explode
void EnemyPurple::hit() {
    dead = true;
}
	
// draw image to display of enemy ship
void EnemyPurple::draw() {
    if (!dead) {
        enemySprite->draw_tinted(centre, color, 0);
    }
    else {
        // enemy has been hit and killed, proceed through death animation sequence
        if (dAnim < 5) deathAnim();
        else dAnim_complete = true;
    }      
}

void EnemyPurple::deathAnim() {  
   deathSprite->draw_death_anim(dAnim, centre, 0);
   dAnim++; // move forward to the next frame of the animation for the next call
}

// update position of enemy ships
void EnemyPurple::update() {

    centre = centre + speed * (*_dt);

    if (centre.x < 0) {
      dead = true;
    }
   
   // check y bound and adjust if out
   if (centre.y > 600 - EnemyPurple_SIZE &&speed.y > 0)  
      speed.reflectY();
   if (centre.y < 0 - EnemyPurple_SIZE && speed.y < 0)
      speed.reflectY();


}
