#include "textureManager.h"

using namespace std;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
   map<string, GLuint>::iterator i;

   for(i = textures.begin(); i != textures.end(); i++)
      glDeleteTextures(1, &i->second);

   for(i = textures.begin(); i != masks.end(); i++)
      glDeleteTextures(1, &i->second);
}

Uint32 TextureManager::getPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

Uint32 TextureManager::setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

void TextureManager::createMask(SDL_Surface *image)
{
   SDL_LockSurface(image);
   int color_black = SDL_MapRGB(image->format, 0xff, 0xff, 0xff);
   int color_white = SDL_MapRGB(image->format, 0x00, 0x00, 0x00);

   for(int i = 0; i < image->w; i++)
   {
      for(int j = 0; j < image->h; j++)
      {
         if(getPixel(image,i,j) != 0)
            setPixel(image, i, j, color_white);
         else
            setPixel(image, i, j, color_black);
      }
   }

   SDL_UnlockSurface(image);
}

GLuint TextureManager::loadTexture(std::string file_name, bool mipmap, bool masking)
{
   glPushAttrib (GL_ALL_ATTRIB_BITS);
   GLuint texture;
   SDL_Surface* imgFile = IMG_Load(file_name.c_str());

   if(imgFile == NULL)
      return -1; 

   if(masking)
      createMask(imgFile);

   glGenTextures(1,&texture);
   glBindTexture(GL_TEXTURE_2D,texture);

   if(mipmap)
   {   
      // Load mipmap texture
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgFile->w, imgFile->h, GL_RGB, GL_UNSIGNED_BYTE, imgFile->pixels);
   }   
   else
   {   
      // Load normal texture
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexImage2D (GL_TEXTURE_2D, 0, 3, imgFile->w, imgFile->h, 0, GL_RGB, GL_UNSIGNED_BYTE, imgFile->pixels);
   }   
   glPopAttrib (); 

   SDL_FreeSurface( imgFile );

   return texture;
}

GLuint TextureManager::addTexture(string file_name, bool mimpmap)
{
   GLuint result;

   if(textures.find(file_name) == textures.end())
      result = loadTexture(file_name, mimpmap, false);
   else
      result = textures[file_name];

   return result;
}

GLuint TextureManager::addMask(string file_name, bool mimpmap)
{
   GLuint result;

   if(masks.find(file_name) == masks.end())
      result = loadTexture(file_name, mimpmap, true);
   else
      result = masks[file_name];

   return result;
}
