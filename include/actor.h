#ifndef _actor_h
#define _actor_h

#include <iostream>
#include <string>

class Actor
{
   protected:
      float x, y;

   public:
      Actor();
      virtual void initialize();
      virtual void update();
};


#endif
