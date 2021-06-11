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
		static void LoadMesh(
			const std::string& path,
			std::vector<glm::vec3>& positions_out,
			std::vector<glm::vec3>& normals_out,
			std::vector<glm::vec2>& textureCoordinates_out,
			std::vector<Submesh>& submeshes_out
		);
		
		static void LoadMesh(const std::string& path,
			std::vector<glm::vec3>& positions_out,
			std::vector<glm::vec3>& normals_out,
			std::vector<glm::vec2>& textureCoordinates_out,
			std::vector<glm::vec3>& tangent_out,
			std::vector<glm::vec3>& bitangent_out,
			std::vector<Submesh>& submeshes_out
		);
	};

}