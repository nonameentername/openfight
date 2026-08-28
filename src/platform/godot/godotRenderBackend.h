#ifndef OPENFIGHT_GODOT_RENDER_BACKEND_H
#define OPENFIGHT_GODOT_RENDER_BACKEND_H

#include "renderBackend.h"
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <map>

namespace godot {

class GodotRenderBackend : public RenderBackend {
private:
    CanvasItem *canvas_item;
    std::map<unsigned int, Ref<Texture2D>> textures;
    unsigned int next_texture_id;
    float logical_width;
    float logical_height;

    Ref<Texture2D> getTexture(unsigned int texture) const;

public:
    GodotRenderBackend();

    void setCanvasItem(CanvasItem *canvas_item);

    void initialize(int width, int height) override;
    void resizeWindow(int width, int height) override;
    void beginFrame() override;
    unsigned int loadTexture(const std::string &file_name, bool mipmap, bool masking) override;
    void releaseTexture(unsigned int texture) override;
    void drawSprite(unsigned int texture, unsigned int mask, bool has_mask, float width, float height, float x, float y,
                    float scale, bool flip, RenderColor color) override;
    void drawCollisionBox(float x, float y, float x_pos, float y_pos, float width, float height, bool inverted,
                          RenderColor color) override;
};

} // namespace godot

#endif
