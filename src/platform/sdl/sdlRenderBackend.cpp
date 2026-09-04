#include "sdlRenderBackend.h"

#include "graphicsCore.h"
#include <SDL2/SDL_image.h>

SdlRenderBackend::SdlRenderBackend(SDL_Renderer *renderer) : renderer(renderer), next_texture_id(1) {
}

SdlRenderBackend::~SdlRenderBackend() {
    for (auto &entry : textures)
        SDL_DestroyTexture(entry.second);
}

void SdlRenderBackend::initialize(int width, int height) {
    resizeWindow(width, height);
}

void SdlRenderBackend::resizeWindow(int width, int height) {
    (void)width;
    (void)height;
    SDL_RenderSetLogicalSize(renderer, 200, 100);
}

void SdlRenderBackend::beginFrame() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

uint32_t SdlRenderBackend::getPixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = static_cast<Uint8 *>(surface->pixels) + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        return *p;
    case 2:
        return *reinterpret_cast<Uint16 *>(p);
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *reinterpret_cast<Uint32 *>(p);
    default:
        return 0;
    }
}

void SdlRenderBackend::setPixel(SDL_Surface *surface, int x, int y, uint32_t pixel) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = static_cast<Uint8 *>(surface->pixels) + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        *p = pixel;
        break;
    case 2:
        *reinterpret_cast<Uint16 *>(p) = pixel;
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
        *reinterpret_cast<Uint32 *>(p) = pixel;
        break;
    }
}

void SdlRenderBackend::createMask(SDL_Surface *image) {
    SDL_LockSurface(image);

    for (int x = 0; x < image->w; x++) {
        for (int y = 0; y < image->h; y++) {
            Uint8 red;
            Uint8 green;
            Uint8 blue;
            Uint8 alpha;
            SDL_GetRGBA(getPixel(image, x, y), image->format, &red, &green, &blue, &alpha);

            alpha = (alpha == 0 || (red == 0 && green == 0 && blue == 0)) ? 0 : 255;
            setPixel(image, x, y, SDL_MapRGBA(image->format, red, green, blue, alpha));
        }
    }

    SDL_UnlockSurface(image);
}

unsigned int SdlRenderBackend::loadTexture(const std::string &file_name, bool mipmap, bool masking) {
    (void)mipmap;

    SDL_Surface *loaded_image = IMG_Load(file_name.c_str());
    if (loaded_image == nullptr)
        return 0;

    SDL_Surface *surface = SDL_ConvertSurfaceFormat(loaded_image, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(loaded_image);
    if (surface == nullptr)
        return 0;

    if (masking)
        createMask(surface);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr)
        return 0;

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    unsigned int texture_id = next_texture_id++;
    textures[texture_id] = texture;
    return texture_id;
}

void SdlRenderBackend::releaseTexture(unsigned int texture) {
    auto found = textures.find(texture);
    if (found == textures.end())
        return;

    SDL_DestroyTexture(found->second);
    textures.erase(found);
}

void SdlRenderBackend::drawSprite(unsigned int texture, unsigned int mask, bool has_mask, float width, float height,
                                  float x, float y, float scale, bool flip, RenderColor color) {
    auto found = textures.find(has_mask ? mask : texture);
    if (found == textures.end())
        found = textures.find(texture);
    if (found == textures.end())
        return;

    float draw_width = width * scale;
    float draw_height = height * scale;
    float center_x = 50.0f + camera->getXposition() + x;
    float center_y = 100.0f - (camera->getYposition() + y);

    SDL_FRect dest = {center_x - draw_width / 2.0f, center_y - draw_height / 2.0f, draw_width, draw_height};
    SDL_SetTextureColorMod(found->second, static_cast<Uint8>(color.red * 255.0f),
                           static_cast<Uint8>(color.green * 255.0f), static_cast<Uint8>(color.blue * 255.0f));
    SDL_SetTextureAlphaMod(found->second, static_cast<Uint8>(color.alpha * 255.0f));
    SDL_RenderCopyExF(renderer, found->second, nullptr, &dest, 0.0, nullptr, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void SdlRenderBackend::drawCollisionBox(float x, float y, float x_pos, float y_pos, float width, float height,
                                        bool inverted, RenderColor color) {
    float box_x = inverted ? -(x_pos + width) : x_pos;
    float game_x = 50.0f + camera->getXposition() + x + box_x;
    float game_y = camera->getYposition() + y + y_pos;
    float draw_y = 100.0f - game_y - height;

    SDL_FRect rect = {game_x, draw_y, width, height};
    SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(color.red * 255.0f), static_cast<Uint8>(color.green * 255.0f),
                           static_cast<Uint8>(color.blue * 255.0f), static_cast<Uint8>(color.alpha * 255.0f));
    SDL_RenderDrawRectF(renderer, &rect);
}
