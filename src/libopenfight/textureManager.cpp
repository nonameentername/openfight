#include "textureManager.h"
#include "renderBackend.h"
#include <map>
#include <string>

using namespace std;

TextureManager::TextureManager() : backend(nullptr) {
}

TextureManager::~TextureManager() {
    clear();
}

unsigned int TextureManager::loadTexture(std::string file_name, bool mipmap, bool masking) {
    if (backend == nullptr)
        return 0;

    return backend->loadTexture(file_name, mipmap, masking);
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
    if (backend == nullptr)
        return;

    for (auto &pair : textures)
        backend->releaseTexture(pair.second);

    for (auto &pair : masks)
        backend->releaseTexture(pair.second);

    textures.clear();
    masks.clear();
}

void TextureManager::setRenderBackend(RenderBackend *backend) {
    this->backend = backend;
}
