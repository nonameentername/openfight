#ifndef OPEN_FIGHT_FILE_READER_H
#define OPEN_FIGHT_FILE_READER_H

#include <functional>
#include <string>

using ReadTextFileCallback = std::function<bool(const std::string &, std::string &)>;

#endif
