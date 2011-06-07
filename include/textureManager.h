#ifndef _texture_manager_
#define _texture_manager_

#include <map>
#include <iostream>
#include <string>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

class TextureManager
{
   private:
      std::map<std::string, GLuint> textures;
      std::map<std::string, GLuint> masks;

      GLuint loadTexture(std::string file_name, bool mipmap, bool masking);
      void createMask(SDL_Surface *image);
      Uint32 getPixel(SDL_Surface *surface, int x, int y); 
      Uint32 setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel); 

   public:
      TextureManager();
      ~TextureManager();

      GLuint addTexture(std::string name, bool mimpmap);
      GLuint addMask(std::string name, bool mimpmap);
      void clear();
};

#endif
