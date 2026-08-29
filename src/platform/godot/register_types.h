#ifndef OPENFIGHT_GODOT_REGISTER_TYPES_H
#define OPENFIGHT_GODOT_REGISTER_TYPES_H

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

namespace godot {

void initialize_godot_openfight_module(ModuleInitializationLevel p_level);
void uninitialize_godot_openfight_module(ModuleInitializationLevel p_level);

} // namespace godot

#endif // OPENFIGHT_GODOT_REGISTER_TYPES_H
