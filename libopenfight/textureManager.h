#ifndef OPEN_FIGHT_TEXTURE_MANAGER_H
#define OPEN_FIGHT_TEXTURE_MANAGER_H

#include "gl.h"
#include <cstdint>
#include <iostream>
#include <map>
#include <string>

struct SDL_Surface;

class TextureManager {
private:
    std::map<std::string, GLuint> textures;
    std::map<std::string, GLuint> masks;

    GLuint loadTexture(std::string file_name, bool mipmap, bool masking);
    void createMask(SDL_Surface *image);
    uint32_t getPixel(SDL_Surface *surface, int x, int y);
    void setPixel(SDL_Surface *surface, int x, int y, uint32_t pixel);

public:
    TextureManager();
    ~TextureManager();

    GLuint addTexture(std::string name, bool mimpmap);
    GLuint addMask(std::string name, bool mimpmap);
    void clear();
};

#endif
