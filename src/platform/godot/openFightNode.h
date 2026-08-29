#ifndef OPENFIGHT_GODOT_OPEN_FIGHT_NODE_H
#define OPENFIGHT_GODOT_OPEN_FIGHT_NODE_H

#include "gameContext.h"
#include "godotRenderBackend.h"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/string.hpp>

#include <memory>

namespace godot {

class OpenFightNode : public Node2D {
    GDCLASS(OpenFightNode, Node2D)

private:
    std::unique_ptr<GameContext> game;
    std::unique_ptr<GodotRenderBackend> renderer;
    bool running;
    float update_accumulator_ms;

    String player_one_path;
    String player_two_path;
    String moves_path;
    String background_path;

    static std::string toFilePath(const String &value);
    static std::string toResourcePath(const String &value);
    FrameInput readFrameInput() const;

protected:
    static void _bind_methods();
    void _notification(int what);

public:
    OpenFightNode();
    ~OpenFightNode();

    void setup_game();
    void shutdown_game();

    void set_player_one_path(const String &path);
    String get_player_one_path() const;
    void set_player_two_path(const String &path);
    String get_player_two_path() const;
    void set_moves_path(const String &path);
    String get_moves_path() const;
    void set_background_path(const String &path);
    String get_background_path() const;
};

} // namespace godot

#endif
