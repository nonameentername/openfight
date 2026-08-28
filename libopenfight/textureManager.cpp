#include "textureManager.h"
#include <SDL2/SDL_image.h>
#include <map>
#include <string>

using namespace std;

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
    for (auto &pair : textures)
        glDeleteTextures(1, &pair.second);

    for (auto &pair : masks)
        glDeleteTextures(1, &pair.second);
}

Uint32 TextureManager::getPixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        return *p;
    case 2:
        return *(Uint16 *)p;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;
    default:
        return 0;
    }
}

void TextureManager::setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16 *)p = pixel;
        break;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
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

void TextureManager::createMask(SDL_Surface *image) {
    SDL_LockSurface(image);
    Uint32 color_black = SDL_MapRGB(image->format, 0xff, 0xff, 0xff);
    Uint32 color_white = SDL_MapRGB(image->format, 0x00, 0x00, 0x00);

    for (int x = 0; x < image->w; x++) {
        for (int y = 0; y < image->h; y++) {
            if (getPixel(image, x, y) != 0)
                setPixel(image, x, y, color_white);
            else
                setPixel(image, x, y, color_black);
        }
    }

    SDL_UnlockSurface(image);
}

GLuint TextureManager::loadTexture(std::string file_name, bool mipmap, bool masking) {
    GLuint texture;
    SDL_Surface *imgFile = IMG_Load(file_name.c_str());

    if (imgFile == nullptr)
        return 0;

    if (masking)
        createMask(imgFile);

    GLenum texture_format = GL_RGB;
    if (imgFile->format->BytesPerPixel == 4)
        texture_format = GL_RGBA;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload texture to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, imgFile->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, imgFile->w, imgFile->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, imgFile->pixels);

    if (mipmap)
        glGenerateMipmap(GL_TEXTURE_2D); // Requires OpenGL >= 3.0 or EXT_framebuffer_object

    SDL_FreeSurface(imgFile);

    return texture;
}

GLuint TextureManager::addTexture(string file_name, bool mipmap) {
    if (textures.find(file_name) == textures.end())
        textures[file_name] = loadTexture(file_name, mipmap, false);

    return textures[file_name];
}

GLuint TextureManager::addMask(string file_name, bool mipmap) {
    if (masks.find(file_name) == masks.end())
        masks[file_name] = loadTexture(file_name, mipmap, true);

    return masks[file_name];
}
