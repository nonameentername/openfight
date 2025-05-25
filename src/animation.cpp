#include "animation.h"
#include "player.h"

using namespace std;

Animation::Animation(bool is_combo, bool is_continual, bool show_hitbox)
{
   this->is_combo     = is_combo;
   this->is_continual = is_continual;
   this->show_hitbox  = show_hitbox;
   current       = 0;
   current_delay = 0;
}

Animation::Animation(const Animation &a)
{
   current       = a.current;
   current_delay = a.current_delay;
   is_combo      = a.is_combo;
   is_continual  = a.is_continual;
   inverted      = a.inverted;
   x_pos         = a.x_pos;
   y_pos         = a.y_pos;
   scale         = a.scale;

   for(int i = 0; i < a.sprites.size(); i++)
   {
      sprites.push_back(new Sprite(*a.sprites[i]));
      offense.push_back(new Collision(*a.offense[i]));
      defense.push_back(new Collision(*a.defense[i]));
      action.push_back(new Actions(*a.action[i]));
   }

   delays = a.delays;
}

Animation::~Animation()
{
   for(int i = 0; i < sprites.size(); i++)
   {
      delete sprites[i];
   }
   for(int i = 0; i < offense.size(); i++)
   {
      delete offense[i];
   }
   for(int i = 0; i < defense.size(); i++)
   {
      delete defense[i];
   }
   for(int i = 0; i < action.size(); i++)
   {
      delete action[i];
   }
}

void Animation::updateCollisions()
{
   GLfloat width  = inverted ? sprites[current]->getWidth()/2 : - sprites[current]->getWidth()/2;
   GLfloat height = sprites[current]->getHeight();

   offense[current]->update(x_pos + width, y_pos - height/2, inverted);
   defense[current]->update(x_pos + width, y_pos - height/2, inverted);
}

GLfloat Animation::getWidth()
{
   return sprites[current]->getWidth();
}

GLfloat Animation::getHeight()
{
   return sprites[current]->getHeight();
}

Collision *Animation::getOffense()
{
   updateCollisions();
   return offense[current];
}

Collision *Animation::getDefense()
{
   updateCollisions();
   return defense[current];
}

Actions *Animation::getActions()
{
   return action[current];
}

bool Animation::isCombo()
{
   return is_combo;
}

bool Animation::isContinual()
{
   return is_continual;
}

void Animation::addFrame(string file_name, GLfloat w, GLfloat h, Collision *o, Collision *d, Actions *a, uint delay)
{
   Sprite *sprite = new Sprite();
   GLuint texture = texture_manager->addTexture(file_name, false);
   GLuint mask    = texture_manager->addMask(file_name, false);

   sprite->setTexture(texture, w, h);
   sprite->setMask(mask);

   sprites.push_back(sprite);
   offense.push_back(o);
   defense.push_back(d);
   action.push_back(a);
   delays.push_back(delay);
}

void Animation::update(GLfloat x, GLfloat y, GLfloat s, bool flip)
{
   x_pos    = x;
   y_pos    = y;
   scale    = s;
   inverted = flip;
}

bool Animation::nextFrame(bool &changed)
{
   changed = false;

   if(delays[current] != 0 && 
      delays[current] < current_delay)
   {
      changed = true;
      current_delay = 0;
      current++;

      if(current >= sprites.size())
      {
         current = 0;
         return true;
      }
   }
   else
   {
      current_delay++;
   }

   return false;
}

void Animation::restart()
{
   current       = 0;
   current_delay = 0;
}

void Animation::draw(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
   updateCollisions();
   sprites[current]->draw(x_pos, y_pos, scale, inverted, red, green, blue, alpha);
   drawHitBox();
}

void Animation::draw(GLfloat alpha)
{
   updateCollisions();
   sprites[current]->draw(x_pos, y_pos, scale, inverted, alpha);
   drawHitBox();
}

void Animation::draw()
{
   updateCollisions();
   sprites[current]->draw(x_pos, y_pos, scale, inverted);
   drawHitBox();
   
}

void Animation::drawHitBox(){
   if(!show_hitbox) return;
   offense[current]->draw();
   defense[current]->draw();
}

