#include "graphics.h"
#include "graphicsCore.h"

Graphics::Graphics() {
    camera = new Camera();
}

Graphics::~Graphics() {
    delete camera;
}

void Graphics::initialize(int w, int h, RenderBackend &renderer) {
    width = w;
    height = h;

    renderer.initialize(w, h);
}

void Graphics::resizeWindow(int w, int h, RenderBackend &renderer) {
    width = w;
    height = h;

    renderer.resizeWindow(w, h);
}

int Graphics::getScreenWidth() {
    return width;
}

int Graphics::getScreenHeight() {
    return height;
}
