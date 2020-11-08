#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

bool loadOBJ(
	const char* path,
	std::vector<float>& out_vertices,
	std::vector<uint32_t>& out_indices
);