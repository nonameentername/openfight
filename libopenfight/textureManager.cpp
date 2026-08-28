#include "textureManager.h"
#include "graphicsCore.h"
#include <map>
#include <string>

using namespace std;

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
    clear();
}

unsigned int TextureManager::loadTexture(std::string file_name, bool mipmap, bool masking) {
    if (render_backend == nullptr)
        return 0;

    return render_backend->loadTexture(file_name, mipmap, masking);
}

unsigned int TextureManager::addTexture(string file_name, bool mipmap) {
    if (textures.find(file_name) == textures.end())
        textures[file_name] = loadTexture(file_name, mipmap, false);

    return textures[file_name];
}

unsigned int TextureManager::addMask(string file_name, bool mipmap) {
    if (masks.find(file_name) == masks.end())
        masks[file_name] = loadTexture(file_name, mipmap, true);

    return masks[file_name];
}

void TextureManager::clear() {
    if (render_backend == nullptr)
        return;

    for (auto &pair : textures)
        render_backend->releaseTexture(pair.second);

    for (auto &pair : masks)
        render_backend->releaseTexture(pair.second);

    textures.clear();
    masks.clear();
}
