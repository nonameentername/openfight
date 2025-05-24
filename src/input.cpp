#include "input.h"

using namespace std;

string Input::key_names[KEY_MAX] =
{ 
   "U",
   "D",
   "B",
   "F",
   "a",
   "b",
   "c",
   "x",
   "y",
   "z"
};

Input::Input()
{
   for(int i =0; i < KEY_MAX; i++)
      keys[i] = false;

   quit_key = false;

   int num_joysticks = SDL_NumJoysticks();
   vector<SDL_Joystick*> sticks;

   for(int i = 0; i < num_joysticks; i++)
      sticks.push_back(SDL_JoystickOpen(i));
}

Input::~Input()
{
   for(int i = 0; i < playerKeys.size(); i++)
      delete playerKeys[i];
   
   for(int i = 0; i < sticks.size(); i++)
      SDL_JoystickClose(sticks[i]);
}

string Input::deviceName(SDL_Event event)
{
   stringstream sstream;

   switch(event.type)
   {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
      sstream << "keyboard";
      break;
      case SDL_JOYBUTTONDOWN:
      case SDL_JOYBUTTONUP:
      sstream << "joystick" << (int)event.jbutton.which;
      break;
      case SDL_JOYAXISMOTION:
      sstream << "joystick" << (int)event.jaxis.which;
      break;
   }
   return sstream.str();
}

void Input::addPlayer()
{
   KeyStruct *player = new KeyStruct();
   string device;

   for(int i = 0; i < KEY_MAX; i++)
   {
      cout << key_names[i] << endl;
      player->config_keys[i]   = getKeyWait(device);
      player->config_device[i] = device;
      player->keys[i] = false;
   }

   playerKeys.push_back(player);
}

void Input::addPlayer(int *config_keys, string *config_device)
{
   KeyStruct *player = new KeyStruct();
   
   for(int i = 0; i < KEY_MAX; i++)
   {
      player->config_keys[i]   = config_keys[i];
      player->config_device[i] = config_device[i];
      player->keys[i] = false;
   }

   playerKeys.push_back(player);
}

int Input::getKeyWait(string &device)
{
   SDL_Event event;

   bool done = false;
   int result;
  
   do
   { 
      SDL_PollEvent(&event);

      switch(event.type)
      {
         case SDL_KEYDOWN:
            device = deviceName(event);
            result = event.key.keysym.sym;
            done = true;
            break;
         case SDL_JOYBUTTONDOWN:
            device = deviceName(event);
            result = event.jbutton.button;
            done = true;
            break;
         case SDL_JOYAXISMOTION:
            if(event.jaxis.value > 0)
            {
               device = deviceName(event);
               result = event.jaxis.axis;
               done = true;
            }
      }
   }
   while(!done);

   return result;
}

SDL_Event Input::poll()
{
   SDL_Event event;

   string device;
   int button;
   bool pressed;

   SDL_PumpEvents();

   while(SDL_PollEvent(&event))
   {
      device = "";
      button = -1;

      switch(event.type)
      {
            quit_key = true;
            break;
         case SDL_KEYDOWN:
            device  = deviceName(event);
            button  = event.key.keysym.sym;
            pressed = true;
            break;
         case SDL_KEYUP:
            device  = deviceName(event);
            button  = event.key.keysym.sym;
            pressed = false;
            break;
         case SDL_JOYBUTTONDOWN:
            device  = deviceName(event);
            button  = event.jbutton.button;
            pressed = true;
            break;
         case SDL_JOYBUTTONUP:
            device  = deviceName(event);
            button  = event.jbutton.button;
            pressed = false;
            break;
         case SDL_JOYAXISMOTION:
            device  = deviceName(event);
            button  = event.jaxis.axis;
            pressed = event.jaxis.value > 0;
            break;
         case SDL_VIDEORESIZE:
            return event;
      }

      if(device == "keyboard" &&
         button == SDLK_ESCAPE)
         quit_key = true;

      for(int i = 0; i < playerKeys.size(); i++)
         for(int j = 0; j < KEY_MAX; j++)
            if(device == playerKeys[i]->config_device[j] &&
               button == playerKeys[i]->config_keys[j])
            {
               playerKeys[i]->keys[j] = pressed;
            }
   }
   return event;
}

string Input::getKeyName(int key)
{
   return key_names[key];
}

bool Input::quitGame()
{
   return quit_key;
}

bool *Input::getKeys(int player)
{
   for(int i = 0; i < KEY_MAX; i++)
      keys[i] = playerKeys[player]->keys[i];

   return keys;
}



