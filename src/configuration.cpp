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

void Configuration::read()
{
   xmlDoc  *doc          = NULL;
   xmlNode *root_element = NULL;
   xmlNode *player_node  = NULL;
   xmlNode *current_node = NULL;

   doc = xmlReadFile(file_name.c_str(), NULL, 0);

   if(doc == NULL)
      return;

   root_element = xmlDocGetRootElement(doc);

   player_node  = findChild(root_element, config_name[CONFIG_PLAYER_ONE]);
   if(player_node != NULL)
   {
      int j = 0;
      for(int i = CONFIG_KEY_UP; i <= CONFIG_KEY_Z; i++)
      {
         current_node = findChild(player_node, config_name[i]);

         if(current_node != NULL)
         {
            config_keys_one[j] = getProperty<int>(current_node, "key");
            device_keys_one[j] = getProperty<string>(current_node, "device");
            j++;
         }
      }
   }

   player_node  = findChild(root_element, config_name[CONFIG_PLAYER_TWO]);

   if(player_node != NULL)
   {
      int j = 0;
      for(int i = CONFIG_KEY_UP; i <= CONFIG_KEY_Z; i++)
      {
         current_node = findChild(player_node, config_name[i]);

         if(current_node != NULL)
         {
            config_keys_two[j] = getProperty<int>(current_node, "key");
            device_keys_two[j] = getProperty<string>(current_node, "device");
            j++;
         }
      }
   }

   xmlFreeDoc(doc);
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

