#include "godotRenderBackend.h"

#include "graphicsCore.h"

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/rect2.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

GodotRenderBackend::GodotRenderBackend()
    : canvas_item(nullptr), next_texture_id(1), logical_width(200.0f), logical_height(100.0f) {
}

void GodotRenderBackend::setCanvasItem(CanvasItem *canvas_item) {
    this->canvas_item = canvas_item;
}

void GodotRenderBackend::initialize(int width, int height) {
    (void)width;
    (void)height;
}

void GodotRenderBackend::resizeWindow(int width, int height) {
    (void)width;
    (void)height;
}

void GodotRenderBackend::beginFrame() {
}

Ref<Texture2D> GodotRenderBackend::getTexture(unsigned int texture) const {
    auto found = textures.find(texture);
    if (found == textures.end())
        return Ref<Texture2D>();

    return found->second;
}

unsigned int GodotRenderBackend::loadTexture(const std::string &file_name, bool mipmap, bool masking) {
    (void)mipmap;
    (void)masking;

    ResourceLoader *loader = ResourceLoader::get_singleton();
    if (loader == nullptr)
        return 0;

    String path = String::utf8(file_name.c_str());
    Ref<Resource> resource = loader->load(path);

    if (resource.is_null() && file_name.rfind("res://", 0) != 0 && file_name.rfind("user://", 0) != 0) {
        resource = loader->load(String("res://") + path);
    }

    Ref<Texture2D> texture = resource;
    if (texture.is_null())
        return 0;

    if (masking) {
        Ref<Image> image = texture->get_image();
        if (image.is_null() || image->is_empty())
            return 0;

        image->convert(Image::FORMAT_RGBA8);
        for (int row = 0; row < image->get_height(); row++) {
            for (int column = 0; column < image->get_width(); column++) {
                Color pixel = image->get_pixel(column, row);
                if (pixel.r == 0.0f && pixel.g == 0.0f && pixel.b == 0.0f)
                    pixel.a = 0.0f;
                else
                    pixel.a = 1.0f;

                image->set_pixel(column, row, pixel);
            }
        }

        texture = ImageTexture::create_from_image(image);
        if (texture.is_null())
            return 0;
    }

    unsigned int texture_id = next_texture_id++;
    textures[texture_id] = texture;
    return texture_id;
}

void GodotRenderBackend::releaseTexture(unsigned int texture) {
    textures.erase(texture);
}

void GodotRenderBackend::drawSprite(unsigned int texture, unsigned int mask, bool has_mask, float width, float height,
                                    float x, float y, float scale, bool flip, RenderColor color) {
    if (canvas_item == nullptr)
        return;

    Ref<Texture2D> godot_texture = has_mask ? getTexture(mask) : getTexture(texture);
    if (godot_texture.is_null())
        godot_texture = getTexture(texture);

    if (godot_texture.is_null())
        return;

    float draw_width = width * scale;
    float draw_height = height * scale;
    float center_x = camera->getXposition() + x;
    float center_y = logical_height - (camera->getYposition() + y);

    Rect2 rect(Vector2(-draw_width / 2.0f, -draw_height / 2.0f), Vector2(draw_width, draw_height));
    Color modulate(color.red, color.green, color.blue, color.alpha);

    if (flip)
        canvas_item->draw_set_transform(Vector2(center_x, center_y), 0.0f, Vector2(-1.0f, 1.0f));
    else
        canvas_item->draw_set_transform(Vector2(center_x, center_y));

    canvas_item->draw_texture_rect(godot_texture, rect, false, modulate);
    canvas_item->draw_set_transform(Vector2());
}

void GodotRenderBackend::drawCollisionBox(float x, float y, float x_pos, float y_pos, float width, float height,
                                          bool inverted, RenderColor color) {
    if (canvas_item == nullptr)
        return;

    float box_x = inverted ? -(x_pos + width) : x_pos;
    float game_x = camera->getXposition() + x + box_x;
    float game_y = camera->getYposition() + y + y_pos;
    float draw_y = logical_height - game_y - height;

    Rect2 rect(Vector2(game_x, draw_y), Vector2(width, height));
    Color line_color(color.red, color.green, color.blue, color.alpha);
    canvas_item->draw_rect(rect, line_color, false, 0.5f);
}
