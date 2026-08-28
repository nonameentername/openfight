#include "sdlGlRenderBackend.h"
#include "gl.h"
#include "graphicsCore.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

void SdlGlRenderBackend::initialize(int width, int height) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);

    resizeWindow(width, height);
}

void SdlGlRenderBackend::beginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-50.0f, -50.0f, -120.0f);
}

void SdlGlRenderBackend::resizeWindow(int width, int height) {
    if (height == 0)
        height = 1;

    GLfloat ratio = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
    glViewport(0, 0, static_cast<GLint>(width), static_cast<GLint>(height));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, ratio, 0.1f, 200.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

uint32_t SdlGlRenderBackend::getPixel(SDL_Surface *surface, int x, int y) {
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
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *reinterpret_cast<Uint32 *>(p);
    default:
        return 0;
    }
}

void SdlGlRenderBackend::setPixel(SDL_Surface *surface, int x, int y, uint32_t pixel) {
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

void SdlGlRenderBackend::createMask(SDL_Surface *image) {
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

unsigned int SdlGlRenderBackend::loadTexture(const std::string &file_name, bool mipmap, bool masking) {
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

    glTexImage2D(GL_TEXTURE_2D, 0, imgFile->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, imgFile->w, imgFile->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, imgFile->pixels);

    if (mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(imgFile);

    return texture;
}

void SdlGlRenderBackend::releaseTexture(unsigned int texture) {
    GLuint gl_texture = texture;
    glDeleteTextures(1, &gl_texture);
}

void SdlGlRenderBackend::drawSprite(unsigned int texture, unsigned int mask, bool has_mask, float width, float height,
                                    float x, float y, float scale, bool flip, RenderColor color) {
    glPushMatrix();

    int tX = width / 2.0f * scale;
    int tY = height / 2.0f * scale;

    if (flip)
        tX = -tX;

    glTranslatef(camera->getXposition() + x, camera->getYposition() + y, camera->getZposition());

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    if (has_mask && color.alpha == 1) {
        glBlendFunc(GL_DST_COLOR, GL_ZERO);
        glBindTexture(GL_TEXTURE_2D, mask);

        glBegin(GL_QUADS);
        glColor4f(color.red, color.green, color.blue, color.alpha);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3i(-tX, -tY, 0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3i(-tX, tY, 0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3i(tX, tY, 0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3i(tX, -tY, 0);
        glEnd();
    }

    glBlendFunc(GL_ONE, GL_ONE);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glColor4f(color.red, color.green, color.blue, color.alpha);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3i(-tX, -tY, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3i(-tX, tY, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3i(tX, tY, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3i(tX, -tY, 0);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
}

void SdlGlRenderBackend::drawCollisionBox(float x, float y, float x_pos, float y_pos, float width, float height,
                                          bool inverted, RenderColor color) {
    glPushMatrix();

    int tX = width / 2.0f;
    int tY = height / 2.0f;

    GLfloat tXi = inverted ? -tX : tX;
    GLfloat x_posi = inverted ? -x_pos : x_pos;

    glTranslatef(camera->getXposition() + x + x_posi + tXi, camera->getYposition() + y + y_pos + tY,
                 camera->getZposition());
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glLineWidth(2);
    glColor4f(color.red, color.green, color.blue, color.alpha);

    glBegin(GL_LINE_LOOP);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3i(-tX, -tY, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3i(-tX, tY, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3i(tX, tY, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3i(tX, -tY, 0);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glPopMatrix();
}
