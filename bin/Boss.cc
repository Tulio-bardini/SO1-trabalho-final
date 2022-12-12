/**
 * @file Boss.cc
 * @brief Implementation of Boss class
 *
 * @author
 * @bug
 **/
#include "Boss.h"

__BEGIN_API

// boss size is 60-80 depending on damage level-- set to one off the start so the boss is
//  "invulnerable" while entering the screen.
#define BOSS_HP 50
#define DELAY_LASER_1 150
#define DELAY_LASER_2 250
#define DELAY_MISSILE 70
#define SIZE_BOSS 70
#define TypeClassNumber 2

Boss::Boss(Ship *ship, std::list<std::shared_ptr<Missile>> *missiles,
           float *dt, std::list<Laser> *lasers, bool *finish) : speed(Vector(-100, 0)),
                                                                lives(30), dAnim(0),
                                                                dAnim_complete(false), fire(true)
{
    hitbox = 1;
    targetable = false;
    col = 0;
    row = 0;
    centre = Point(1100, 300);
    _dt = dt;
    _finish = finish;
    lives = BOSS_HP;
    _lasers = lasers;
    _ship = ship;
    _missiles = missiles;
    size = SIZE_BOSS;

    setTimers();
    load_sprite();
}

void Boss::load_sprite()
{

    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(path, "resources");
    al_change_directory(al_path_cstr(path, '/'));

    bossSprite = std::make_shared<Sprite>("bossv1.png"); // espaçonave do enimigo
    bossDeath = std::make_shared<Sprite>("explode.png");
}

Boss::~Boss()
{
    fireLaserDelay1.reset();
}

// When hit, decrement lives by 1
void Boss::hit()
{
    
    if (lives > 1)
    {
        lives -= 1;
    }
    else {
        dead = true;
        aliveBoss = false;
    }

}

// draw image to display the boss ship
void Boss::draw()
{
    chooseFrame();
    if (!dead)
    {
        bossSprite->draw_boss(0, col, 200, 200, centre, 0);
    }
    else
    {
        // Boss is dead and we proceed to the death animation
        if (dAnim < 5)
            deathAnim();
        else
            dAnim_complete = true;
    }
}

// this function is going to be a little interesting
void Boss::deathAnim()
{
    bossDeath->draw_death_anim(dAnim, centre, 0);
    dAnim++;
}

// update Boss movement
void Boss::update()
{
    // update position
    centre = centre + speed * (*_dt);

    if (centre.x < 700 && !targetable)
    {
        targetable = true; // becomes targetable
        hitbox = 80;       // set hitbox to 80
        speed.x = 0;       // no more x movement
        speed.y = 100;     // begin downward movement
    }
    // reflect conditions
    if (centre.y > 450 && speed.y > 0)
    { // if below mark and moving downwards
        speed.reflectY();
    }
    if (centre.y < 150 && speed.y < 0)
    { // if above mark and moving upwards
        speed.reflectY();
    }
    // check fire condition
    if (fireLaserDelay1->getCount() > DELAY_LASER_1)
    {
        fireLaser1();
        fireLaserDelay1->srsTimer(); // stop, reset, start
    }

    if (fireLaserDelay2->getCount() > DELAY_LASER_2)
    {
        fireLaser2();
        fireLaserDelay2->srsTimer(); // stop, reset, start
    }

    if (fireMissileDelay->getCount() > DELAY_MISSILE)
    {
        fireMissile();
        fireMissileDelay->srsTimer(); // stop, reset, start
    }
}

// for animation
void Boss::chooseFrame()
{
    // middle damage animation--fire speed goes up.
    if (lives <= 35 && col == 0)
    {
        fireSpeed = rand() % 50 + 20;
        speed = speed * 1.1; // increase speed
        hitbox = 70;
        col = 1;
    }
    // final damage animation-- fire speed up again
    if (lives <= 20 && col == 1)
    {
        fireSpeed = rand() % 30 + 20;
        speed = speed * 1.1; // increase speed
        hitbox = 60;
        col = 2;
    }
}

void Boss::setBossAlive()
{
    centre = Point(1100, 300);
    aliveBoss = true;
}

bool Boss::getBossAlive()
{
    return aliveBoss;
}

void Boss::setTimers()
{
    fireLaserDelay1 = std::make_shared<Timer>(60);
    fireLaserDelay1->create();
    fireLaserDelay1->startTimer();
    fireLaserDelay2 = std::make_shared<Timer>(60);
    fireLaserDelay2->create();
    fireLaserDelay2->startTimer();
    fireMissileDelay = std::make_shared<Timer>(60);
    fireMissileDelay->create();
    fireMissileDelay->startTimer();
}

void Boss::fireLaser1()
{

    for (int i = 100; i > -100; i -= 20)
    {
        _lasers->push_back(Laser(Point(centre.x, centre.y + i), al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(-500, 0)));
    }
}

void Boss::fireLaser2()
{
    Vector aim(0, 0);
    Point shipLock = _ship->centre;
    aim.Angle(shipLock, centre, 0.9);

    for (int i = -70; i <= 70; i += 20)
    {
        _lasers->push_back(Laser(centre + Point(50, 0), al_map_rgb(200, 0, 0), TypeClassNumber, _dt, aim + Vector(-30, i)));
    }
}

void Boss::fireMissile()
{
    Vector aim(0, 0);
    Point shipLock = _ship->centre;

    aim.Angle(shipLock, centre + Point(0, 50), 0.9);
    double ship_angle = atan(aim.y/aim.x)+4.71;
    _missiles->push_back(std::make_shared<Missile> (centre + Point(0, 50), aim, ship_angle, TypeClassNumber, _dt));
    aim.Angle(shipLock, centre + Point(0, -50), 0.9);
    ship_angle = atan(aim.y/aim.x)+4.71;
    _missiles->push_back(std::make_shared<Missile> (centre + Point(0, -50), aim, ship_angle, TypeClassNumber, _dt));
}

void Boss::restart()
{
    dead = false;
    hitbox = 1;
    targetable = false;
    col = 0;
    row = 0;
    centre = Point(1100, 300);
    lives = BOSS_HP;
    size = SIZE_BOSS;
    speed = Vector(-100, 0);

}

void Boss::run()
{

    while (!*_finish)
    {
        if (aliveBoss && !dead)
        {
            update();
            chooseFrame();
        }
        Thread::yield();
    }

    Thread::running()->thread_exit(0);
}

__END_API
