#ifndef _key_state_h
#define _key_state_h

#include <iostream>
#include <string>
#include <list>
#include <SDL2/SDL.h>
#include "input.h"

class KeyState
{
   private:
      int key;
      int time_held;
      bool pressed;
      bool held;

   public:
      KeyState(int key, bool pressed);
      KeyState(int key, int time_held, bool pressed, bool held);
      ~KeyState();

      bool operator==(const KeyState *c) const;
      void   setTimeHeld(int time_held);
      void   setHeld(bool held);
      int    getKey();
      bool   keyPressed();
};

#endif
