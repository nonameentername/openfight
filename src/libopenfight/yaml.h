#pragma once

#include <fstream>
#include <string>
#include <yaml-cpp/yaml.h>
using namespace YAML;
using namespace std;

#include "fileReader.h"

Node loadYamlFile(const std::string &file_name, const ReadTextFileCallback &read_text_file = ReadTextFileCallback());