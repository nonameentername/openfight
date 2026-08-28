#ifndef OPEN_FIGHT_TEXTURE_MANAGER_H
#define OPEN_FIGHT_TEXTURE_MANAGER_H

#include <iostream>
#include <map>
#include <string>

class RenderBackend;

class TextureManager {
private:
    RenderBackend *backend;
    std::map<std::string, unsigned int> textures;
    std::map<std::string, unsigned int> masks;

    unsigned int loadTexture(std::string file_name, bool mipmap, bool masking);

public:
    void setRenderBackend(RenderBackend *backend);
    TextureManager();
    ~TextureManager();

    unsigned int addTexture(std::string name, bool mimpmap);
    unsigned int addMask(std::string name, bool mimpmap);
    void clear();
};

#endif
