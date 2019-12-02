#pragma once
#include "pch.h"

int loadObj(const char* filename, std::vector< glm::vec3 >& outvertex,
	std::vector< glm::vec3 >& outnormal, std::vector< glm::vec2 >& outuv);
