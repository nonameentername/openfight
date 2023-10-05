#include <SDL/SDL.h>
#include "utilities.h"
#include "input.h"
#include "animation.h"
#include "playerAgent.h"
#include "sprite.h"
#include "graphicsCore.h"
#include "graphics.h"
#include "configuration.h"
#include "playerBridge.h"

using namespace std;

#define screen_width  800
#define screen_height 600
#define screen_bpp    24

#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char *argv[])
{
   if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == 1 )
      return -1;

   const SDL_VideoInfo *video_info = SDL_GetVideoInfo();
   if(video_info == NULL)
      return -1;

   atexit(SDL_Quit);

   int video_flags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_RESIZABLE;

   if(video_info->hw_available)
      video_flags |= SDL_HWSURFACE;
   else
      video_flags |= SDL_SWSURFACE;

   if(video_info->blit_hw)
      video_flags |= SDL_HWACCEL;

   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

   SDL_SetVideoMode(screen_width, screen_height, screen_bpp, video_flags);

   graphics->initialize(screen_width, screen_height);

   Configuration configuration("data/config.xml");
   configuration.read();

   Input input;
   input.addPlayer(configuration.getConfigKeys(true), configuration.getConfigDevice(true));
   input.addPlayer(configuration.getConfigKeys(false), configuration.getConfigDevice(false));

   PlayerAgent player;
   player.initialize("data/ryu/ryu.xml", "data/ryu/moves.xml", true);

   PlayerAgent player2;
   player2.initialize("data/ryu/ryu.xml","data/ryu/moves.xml",  false);

   player.getPlayer()->setOpponent(player2.getPlayer());
   player2.getPlayer()->setOpponent(player.getPlayer());

   Sprite background;
   GLuint texture = texture_manager->addTexture("data/background.png", false);
   background.setTexture(texture,200,100);

   PlayerBridge bridge;
   bridge.initialize(player.getPlayer(), player2.getPlayer());

   object_manager->add("0", player.getPlayer());
   object_manager->add("1", player2.getPlayer());


   while(!input.quitGame())
   {  
      float game_time;

      if(updateGame(game_time))
      {
         SDL_Event event = input.poll();
 
         if(event.type == SDL_VIDEORESIZE)
         {
            SDL_SetVideoMode(event.resize.w, event.resize.h, screen_bpp, video_flags);
            graphics->resizeWindow(event.resize.w, event.resize.h);
         }

         bool *keys = input.getKeys(0);
         player.update(keys);

         bool *keys2 = input.getKeys(1);
         player2.update(keys2);

         string key = object_manager->first();
         while(key != "")
         {
            bool done = false;

            if(key != "0" && key != "1")
               done = object_manager->get(key)->update();

            if (done) {
                object_manager->remove(key);
                break;
            }

            key = object_manager->next();
         }

         //draw
         glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // Clear color and depth buffer

         glLoadIdentity(); // Reset orientation
         glTranslatef(-50.0f, -50.0f, -120.0f);

         bridge.update();
         background.draw(50,50,1, false);

         list<Player*> objects = object_manager->getSortedList();

         for(list<Player*>::iterator i = objects.begin(); i != objects.end(); i++)
            (*i)->draw();

         SDL_GL_SwapBuffers();

         game_time = getGameTime();
      }
   }
}
