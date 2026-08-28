#include "graphics.h"
#include "graphicsCore.h"

Graphics::Graphics() {
    camera = new Camera();
}

Graphics::~Graphics() {
    delete camera;
}

void Graphics::initialize(int w, int h) {
    width = w;
    height = h;

    if (render_backend != nullptr)
        render_backend->initialize(w, h);
}

void Graphics::resizeWindow(int w, int h) {
    width = w;
    height = h;

    if (render_backend != nullptr)
        render_backend->resizeWindow(w, h);
}

int Graphics::getScreenWidth() {
    return width;
}

int Graphics::getScreenHeight() {
    return height;
}
