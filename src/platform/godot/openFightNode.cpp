#include "openFightNode.h"

#include "graphicsCore.h"
#include "utilities.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/char_string.hpp>
#include <godot_cpp/variant/string_name.hpp>

using namespace godot;

OpenFightNode::OpenFightNode() : running(false), update_accumulator_ms(0.0f) {
    player_one_path = "res://data/ryu/ryu.yml";
    player_two_path = "res://data/ryu/ryu.yml";
    moves_path = "res://data/ryu/moves.yml";
    background_path = "res://data/background.png";
}

OpenFightNode::~OpenFightNode() {
    shutdown_game();
}

std::string OpenFightNode::toFilePath(const String &value) {
    String path = value;
    if (path.begins_with("res://") || path.begins_with("user://")) {
        ProjectSettings *settings = ProjectSettings::get_singleton();
        if (settings != nullptr)
            path = settings->globalize_path(path);
    }

    return toResourcePath(path);
}

std::string OpenFightNode::toResourcePath(const String &value) {
    CharString utf8 = value.utf8();
    return std::string(utf8.get_data());
}

void OpenFightNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("setup_game"), &OpenFightNode::setup_game);
    ClassDB::bind_method(D_METHOD("shutdown_game"), &OpenFightNode::shutdown_game);

    ClassDB::bind_method(D_METHOD("set_player_one_path", "path"), &OpenFightNode::set_player_one_path);
    ClassDB::bind_method(D_METHOD("get_player_one_path"), &OpenFightNode::get_player_one_path);
    ClassDB::bind_method(D_METHOD("set_player_two_path", "path"), &OpenFightNode::set_player_two_path);
    ClassDB::bind_method(D_METHOD("get_player_two_path"), &OpenFightNode::get_player_two_path);
    ClassDB::bind_method(D_METHOD("set_moves_path", "path"), &OpenFightNode::set_moves_path);
    ClassDB::bind_method(D_METHOD("get_moves_path"), &OpenFightNode::get_moves_path);
    ClassDB::bind_method(D_METHOD("set_background_path", "path"), &OpenFightNode::set_background_path);
    ClassDB::bind_method(D_METHOD("get_background_path"), &OpenFightNode::get_background_path);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "player_one_path"), "set_player_one_path", "get_player_one_path");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "player_two_path"), "set_player_two_path", "get_player_two_path");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "moves_path"), "set_moves_path", "get_moves_path");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "background_path"), "set_background_path", "get_background_path");
}

void OpenFightNode::_notification(int what) {
    switch (what) {
    case NOTIFICATION_READY:
        setup_game();
        break;
    case NOTIFICATION_PROCESS: {
        if (!running || game == nullptr)
            break;

        update_accumulator_ms += static_cast<float>(get_process_delta_time() * 1000.0);
        if (update_accumulator_ms > 250.0f)
            update_accumulator_ms = 250.0f;

        while (update_accumulator_ms >= GAME_TIME_UPDATE_FREQ) {
            FrameInput input = readFrameInput();
            game->update(input, GAME_TIME_UPDATE_FREQ);
            update_accumulator_ms -= GAME_TIME_UPDATE_FREQ;
        }

        queue_redraw();
        break;
    }
    case NOTIFICATION_DRAW:
        if (running && game != nullptr && renderer != nullptr) {
            renderer->beginFrame();
            game->render(*renderer);
        }
        break;
    case NOTIFICATION_EXIT_TREE:
        shutdown_game();
        break;
    default:
        break;
    }
}

FrameInput OpenFightNode::readFrameInput() const {
    static const char *input_actions[2][KEY_MAX] = {
        {"p1_up", "p1_down", "p1_left", "p1_right", "p1_a", "p1_b", "p1_c", "p1_x", "p1_y", "p1_z"},
        {"p2_up", "p2_down", "p2_left", "p2_right", "p2_a", "p2_b", "p2_c", "p2_x", "p2_y", "p2_z"}};

    FrameInput frame_input;
    Input *input = Input::get_singleton();
    InputMap *input_map = InputMap::get_singleton();
    if (input == nullptr || input_map == nullptr)
        return frame_input;

    for (int player = 0; player < 2; player++)
        for (int key = 0; key < KEY_MAX; key++) {
            StringName action(input_actions[player][key]);
            frame_input.players[player][key] = input_map->has_action(action) && input->is_action_pressed(action);
        }

    return frame_input;
}

void OpenFightNode::setup_game() {
    if (running)
        shutdown_game();

    game = std::make_unique<GameContext>();
    renderer = std::make_unique<GodotRenderBackend>();
    renderer->setCanvasItem(this);
    graphics->initialize(static_cast<int>(get_viewport_rect().size.x), static_cast<int>(get_viewport_rect().size.y),
                         *renderer);

    GameSetup setup = {toResourcePath(player_one_path), toResourcePath(player_two_path), toResourcePath(moves_path),
                       toResourcePath(background_path)};

    ReadTextFileCallback read_text_file = [](const std::string &path, std::string &contents) {
        String godot_path = String::utf8(path.c_str());
        Ref<FileAccess> file = FileAccess::open(godot_path, FileAccess::READ);
        if ((file.is_null() || !file->is_open()) && !godot_path.begins_with("res://") &&
            !godot_path.begins_with("user://")) {
            file = FileAccess::open(String("res://") + godot_path, FileAccess::READ);
        }
        if (file.is_null() || !file->is_open())
            return false;

        CharString utf8 = file->get_as_text().utf8();
        contents = std::string(utf8.get_data());
        return true;
    };

    update_accumulator_ms = 0.0f;
    running = game->setup(setup, *renderer, read_text_file);
    set_process(running);
    queue_redraw();
}

void OpenFightNode::shutdown_game() {
    if (game != nullptr)
        game->shutdown();

    renderer.reset();
    game.reset();
    running = false;
    update_accumulator_ms = 0.0f;
    set_process(false);
}

void OpenFightNode::set_player_one_path(const String &path) {
    player_one_path = path;
}

String OpenFightNode::get_player_one_path() const {
    return player_one_path;
}

void OpenFightNode::set_player_two_path(const String &path) {
    player_two_path = path;
}

String OpenFightNode::get_player_two_path() const {
    return player_two_path;
}

void OpenFightNode::set_moves_path(const String &path) {
    moves_path = path;
}

String OpenFightNode::get_moves_path() const {
    return moves_path;
}

void OpenFightNode::set_background_path(const String &path) {
    background_path = path;
}

String OpenFightNode::get_background_path() const {
    return background_path;
}
