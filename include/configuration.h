#ifndef _configuration_h
#define _configuration_h

#include <string>
#include <list>
#include <map>
#include <typeinfo>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "input.h"
#include "utilities.h"

enum Config
{
   CONFIG_PLAYER_ONE,
   CONFIG_PLAYER_TWO,
   CONFIG_KEY_UP,
   CONFIG_KEY_DOWN,
   CONFIG_KEY_LEFT,
   CONFIG_KEY_RIGHT,
   CONFIG_KEY_A,
   CONFIG_KEY_B,
   CONFIG_KEY_C,
   CONFIG_KEY_X,
   CONFIG_KEY_Y,
   CONFIG_KEY_Z,
   CONFIG_MAX

};

class Configuration
{
   private:
      std::string file_name;
      xmlDoc  *doc;
      xmlNode *root_element;

      static std::string config_name[CONFIG_MAX];
      int config_keys_one[KEY_MAX];
      int config_keys_two[KEY_MAX];
      std::string device_keys_one[KEY_MAX];
      std::string device_keys_two[KEY_MAX];

   public:
      Configuration(std::string file_name);
      ~Configuration();

      void read();
      void write();

      int *getConfigKeys(bool player_one);
      std::string *getConfigDevice(bool player_one);

};


#endif
