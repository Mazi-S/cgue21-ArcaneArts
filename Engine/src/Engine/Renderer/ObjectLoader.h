#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <Engine/Core/Base.h>
#include <Engine/Renderer/Mesh.h>

#include <glm/glm.hpp>


namespace Engine {

	class ObjectLoader
	{
	public:
		static bool loadOBJ(
			const char* path,
			std::vector<float>& out_vertices,
			std::vector<uint32_t>& out_indices
		);

		static Ref<Mesh> LoadMesh(const std::string& name, const std::string& path);

	};

}