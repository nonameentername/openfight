#include "utilities.h"

using namespace std;

const float GAME_TIME_UPDATE_FREQ = 10;

float getGameTime()
{
   return SDL_GetTicks();
}

bool updateGame(float last_update)
{
   return (getGameTime() - last_update) > GAME_TIME_UPDATE_FREQ;
}

xmlNode *findChild(xmlNode *parent, std::string name)
{
   xmlNode *cur_node = NULL;

   for(cur_node = parent->children; cur_node; cur_node = cur_node->next)
   {   
      if(cur_node->type == XML_ELEMENT_NODE && name.compare((const char*)cur_node->name) == 0)
         return cur_node;
   }   

   return NULL;
}

vector<xmlNode*> findChildren(xmlNode *parent, std::string name)
{
   vector<xmlNode*> result;
   xmlNode *cur_node = NULL;

   for(cur_node = parent->children; cur_node; cur_node = cur_node->next)
   {   
      if(cur_node->type == XML_ELEMENT_NODE && name.compare((const char*)cur_node->name) == 0)
         result.push_back(cur_node);
   }
   
   return result;
}

template <> float getProperty<>(xmlNode *node, string name)
{
   char *result = NULL;

   if(xmlHasProp(node, (const xmlChar*)name.c_str()))
      result = (char*)xmlGetProp(node, (const xmlChar*)name.c_str());

   return atof(result);
}

template <> int getProperty<>(xmlNode *node, string name)
{
   char *result = NULL;

   if(xmlHasProp(node, (const xmlChar*)name.c_str()))
      result = (char*)xmlGetProp(node, (const xmlChar*)name.c_str());

   return atoi(result);
}

template <> std::string getProperty<>(xmlNode *node, string name)
{
   char *result = NULL;

   if(xmlHasProp(node, (const xmlChar*)name.c_str()))
      result = (char*)xmlGetProp(node, (const xmlChar*)name.c_str());

   if(result == NULL)
      return "";

   return result;
}

template <> bool getProperty<>(xmlNode *node, string name)
{
    char *result = NULL;

    if(xmlHasProp(node, (const xmlChar*)name.c_str())) {
        result = (char*)xmlGetProp(node, (const xmlChar*)name.c_str());

        if(strcmp(result, "true") == 0)
            return true;
    }

    return false;
}

template <> int getContent<>(xmlNode *node)
{
   char *result = (char*)xmlNodeGetContent(node);

   if(result != NULL)
      return atoi(result);

   return 0;
}

template <> string getContent<>(xmlNode *node)
{
   char *result = (char*)xmlNodeGetContent(node);

   if(result != NULL)
      return result;

   return "";
}

vector<string> splitString(string str, string delimeters)
{
   vector<string> result;
   char *i; 

   vector<char> buffer(str.begin(), str.end());
   buffer.push_back('\0');

   i = strtok(&buffer[0], delimeters.c_str());

   while(i != NULL)
   {
      result.push_back(i);
      i = strtok(NULL, delimeters.c_str());
   }

   return result;
}

