#ifndef _player_agent_h
#define _player_agent_h

#include <string>
#include <list>
#include <map>
#include <memory>
#include <vector>
#include "input.h"
#include "keyState.h"
#include "player.h"
#include "moves.h"

class PlayerAgent
{
   private:
      std::shared_ptr<Player> player;
      std::unique_ptr<Moves> moves;
      static const int QUEUE_KEY_MAX   = 20;
      static const int INPUT_FRAME_MAX = 10;
      bool previous_keys[KEY_MAX];
      uint current_frame;
      std::list<KeyState> key_queue;
      std::map<std::string, int> time_held;

   public:
      PlayerAgent();
      virtual ~PlayerAgent() = default;

      std::shared_ptr<Player> getPlayer();
      bool initialize(std::string file_name, std::string moves_file, bool player_one);
      void update( bool *keys );
      void draw();
};


#endif
