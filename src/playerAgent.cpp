#include <iostream>
#include <sstream>
#include <list>
#include "playerAgent.h"


using namespace std;

PlayerAgent::PlayerAgent()
    : player(nullptr), moves(nullptr), current_frame(0) {}

std::shared_ptr<Player> PlayerAgent::getPlayer()
{
   return player;
}

bool PlayerAgent::initialize(string file_name, string moves_file, bool player_one)
{
   GLfloat location = player_one ? 30:70;
   player = std::make_shared<Player>();
   player->initialize(file_name, player_one, location, 0);

   moves = std::make_unique<Moves>();
   moves->initialize(moves_file);

   for(int i = 0; i < KEY_MAX; i++)
      previous_keys[i] = false; 

   return true;
}

void PlayerAgent::update(bool *keys)
{
   string command;
   bool changed = false;

   if(player->isInverted())
      swap(keys[KEY_LEFT], keys[KEY_RIGHT]);

   for(int i = 0; i < KEY_MAX; i++)
   {
      if(time_held[Input::getKeyName(i)])
         time_held[Input::getKeyName(i)]++;
         
      if(keys[i] != previous_keys[i])
      {
         player->state[Input::getKeyName(i)] = keys[i];

         if(keys[i])
            time_held[Input::getKeyName(i)] = 1;

         changed = true;

         current_frame = INPUT_FRAME_MAX;
         KeyState item = KeyState(i, keys[i]);

         key_queue.push_back(item);

         if(key_queue.size() > QUEUE_KEY_MAX)
            key_queue.pop_front();
      }
   }

   command = moves->getTrigger(key_queue, time_held);
   player->updateState(command);

   if(current_frame == 0)
      key_queue.clear();
   else
      current_frame--;

   for(int i = 0; i < KEY_MAX; i++)
   {
      if(!keys[i])
         time_held[Input::getKeyName(i)] = 0;

      previous_keys[i] = keys[i];
   }

   player->update();
}

void PlayerAgent::draw()
{
   player->draw();
}
