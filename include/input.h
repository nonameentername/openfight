#ifndef _input_h
#define _input_h

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <SDL/SDL.h>

enum Keys 
{ 
   KEY_UP,
   KEY_DOWN,
   KEY_LEFT,
   KEY_RIGHT,
   KEY_A,
   KEY_B,
   KEY_C,
   KEY_X,
   KEY_Y,
   KEY_Z,
   KEY_MAX
};

struct KeyStruct
{
   std::string config_device[KEY_MAX];
   int config_keys[KEY_MAX];
   bool keys [KEY_MAX];
};

class Input
{
   private:
      std::vector<KeyStruct*> playerKeys;
      bool keys [KEY_MAX];
      std::vector<SDL_Joystick*> sticks;
      bool quit_key;

      int  getKeyWait(std::string &device);
      static std::string key_names[KEY_MAX];
      std::string deviceName(SDL_Event event);

   public:
      Input();
      ~Input();
      void addPlayer();
      void addPlayer(int *config_keys, std::string *config_device);

      SDL_Event poll();
      bool *getKeys(int player);
      bool  quitGame();
      static std::string getKeyName(int key);
};


#endif
