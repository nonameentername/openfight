#include <iostream>
#include <typeinfo>
#include "configuration.h"

using namespace std;

string Configuration::config_name[CONFIG_MAX] = 
{
   "player_one",
   "player_two",
   "key_up",
   "key_down",
   "key_left",
   "key_right",
   "key_a",
   "key_b",
   "key_c",
   "key_x",
   "key_y",
   "key_z"
};

Configuration::Configuration(string file_name)
{
   this->file_name = file_name;
}

Configuration::~Configuration()
{
}

void Configuration::write()
{

}

int *Configuration::getConfigKeys(bool player_one)
{
   if(player_one)
      return config_keys_one;
   else
      return config_keys_two;

   return 0;
}

string *Configuration::getConfigDevice(bool player_one)
{
   if(player_one)
      return device_keys_one;
   else
      return device_keys_two;

   return 0;
}

