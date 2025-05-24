#include "collision.h"

using namespace std;

Collision::Collision()
{
}

Collision::Collision(const Collision &c)
{
   for(int i = 0; i < c.collisions.size(); i++)
      collisions.push_back(new CollisionBox(*c.collisions[i]));
}

Collision::~Collision()
{
   for(int i = 0; i < collisions.size(); i++)
      delete collisions[i];
}

void Collision::addCollision(CollisionBox *c)
{
   collisions.push_back(c);
}

void Collision::update(GLfloat x, GLfloat y, bool inverted)
{
   for(int i = 0; i < collisions.size(); i++)
   {
      if(collisions[i] != NULL)
         collisions[i]->update(x, y, inverted);
   }
}

bool Collision::overlaps(Collision *other)
{
   if(other == NULL)
      return false;

   bool overlap = false;

   for(int i = 0; i < collisions.size(); i++)
      for(int j = 0; j < other->collisions.size(); j++)
      {
         overlap = overlap || collisions[i]->overlaps(other->collisions[j]);
      }

   return overlap;
}

void Collision::draw()
{
   for(int i = 0; i < collisions.size(); i++)
      collisions[i]->draw();
}
