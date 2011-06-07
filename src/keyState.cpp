#include "keyState.h"

using namespace std;

KeyState::KeyState(int key, bool pressed)
{
   this->key        = key;
   this->time_held  = 0;
   this->pressed    = pressed;
   this->held       = false;
}

KeyState::KeyState(int key, int time_held, bool pressed, bool held)
{
   this->key        = key;
   this->time_held  = time_held;
   this->pressed    = pressed;
   this->held       = held;
}

KeyState::~KeyState()
{
}

void KeyState::setHeld(bool held)
{
   this->held = held;
}

void KeyState::setTimeHeld(int time_held)
{
   this->time_held = time_held;
}

bool KeyState::operator== (const KeyState *c) const
{
   return this->key == c->key && 
          this->pressed == c->pressed && 
          this->time_held >= c->time_held &&
          this->held == c->held;
}

int KeyState::getKey()
{
   return key;
}

bool KeyState::keyPressed()
{
   return pressed;
}

