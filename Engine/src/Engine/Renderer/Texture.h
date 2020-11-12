#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>
#include <glad/glad.h>

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char* imagepath);

namespace Engine {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};
}