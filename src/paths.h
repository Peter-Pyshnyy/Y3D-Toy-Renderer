#pragma once
#include <string>

#ifndef SOURCE_DIR
#define SOURCE_DIR "." // backup — in case you compile without CMake
#endif

static const std::string SHADER_DIR = std::string(SOURCE_DIR) + "/resources/shaders/";
static const std::string MODEL_DIR = std::string(SOURCE_DIR) + "/resources/models/";