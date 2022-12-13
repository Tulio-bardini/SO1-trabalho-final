#include "Missile.h"

__BEGIN_API

Missile::Missile(Point p, Vector s, double angle, int owner, float *dt)
{
   centre = p + speed * 0.1;
   speed = s;
   _dt = dt;
   classOwner = owner;
   _angle = angle;
   load_sprites();
}

Missile::~Missile()
{
}

void Missile::load_sprites()
{
   ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
   al_append_path_component(path, "resources");
   al_change_directory(al_path_cstr(path, '/'));
   missileSprite1 = std::make_shared<Sprite>("m1.png");
   missileSprite2 = std::make_shared<Sprite>("m2.png");
   missileSprite3 = std::make_shared<Sprite>("m3.png");
   missileSprite4 = std::make_shared<Sprite>("m4.png");
   missileSprite5 = std::make_shared<Sprite>("m5.png");
   missileSprite6 = std::make_shared<Sprite>("m6.png");
   missileSprite7 = std::make_shared<Sprite>("m7.png");
   al_destroy_path(path);
}

void Missile::update()
{
   centre = centre + speed * (*_dt);
   if (!in_bound())
   {
      live = false;
   }
}

void Missile::draw()
{

   if (mAnim == 0)
   {
      missileSprite1->draw_rotated(centre, _angle, 0);
   }
   else if (mAnim == 1)
   {
      missileSprite2->draw_rotated(centre, _angle, 0);
   }
   else if (mAnim == 2)
   {
      missileSprite3->draw_rotated(centre, _angle, 0);
   }
   else if (mAnim == 3)
   {
      missileSprite4->draw_rotated(centre, _angle, 0);
   }
   else if (mAnim == 4)
   {
      missileSprite5->draw_rotated(centre, _angle, 0);
   }
   else if (mAnim == 5)
   {
      missileSprite5->draw_rotated(centre, _angle, 0);
   }
   else if (mAnim == 6)
   {
      missileSprite6->draw_rotated(centre, _angle, 0);
   }
   else if (mAnim == 7)
   {
      missileSprite7->draw_rotated(centre, _angle, 0);
   }

   mAnim++;
   if (mAnim > 7)
      mAnim = 0; // go through vector again
}

bool Missile::in_bound()
{
   if ((centre.x > 800) ||
       (centre.x < 0) ||
       (centre.y > 600) ||
       (centre.y < 0))
      return false;
   return true;
}

void Missile::runMissile(std::list<std::shared_ptr<Missile>> *missiles, bool *finish)
{
   while (!*finish)
   {
      if (!missiles->empty())
      {
         for (auto it = missiles->begin();
              it != missiles->end(); ++it)
         {
            if (!(*it)->dead)
            {
               (*it)->update();
            }
         }

         std::list<std::shared_ptr<Missile>> newMissiles;
         for (auto it = missiles->begin(); it != missiles->end(); ++it)
         {
            if ((*it)->live)
            {
               newMissiles.push_back(*it);
            }
         }
         missiles->clear();
         missiles->assign(newMissiles.begin(), newMissiles.end());
      }
      Thread::yield();
   }
   Thread::running()->thread_exit(0);
}

__END_API