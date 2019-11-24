#pragma once
#include "pch.h"

char* get_filedata_to_buf(const char* filename);
bool init_shader_program(GLuint* id, const char* filename_vert, const char* filename_frag);