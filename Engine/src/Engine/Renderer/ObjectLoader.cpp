#include "egpch.h"

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include "ObjectLoader.h"

namespace Engine {

	bool loadOBJ(
		const char* path,
		std::vector<float>& out_vertices,
		std::vector<uint32_t>& out_indices
	) {
		LOG_TRACE("Loading OBJ file {} ...", path);

		std::vector<unsigned int> temp_vertexIndices, temp_uvIndices, temp_normalIndices;
		std::vector<glm::vec3> temp_vertices;
		std::vector<glm::vec2> temp_uvs;
		std::vector<glm::vec3> temp_normals;

		FILE* file = fopen(path, "r");
		if (file == NULL) {
			LOG_WARN("Impossible to open the file! Are you in the right path?");
			getchar();
			return false;
		}

		// Parse obj file
		while (1)
		{
			char lineHeader[128];

			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break;

			// Position
			if (strcmp(lineHeader, "v") == 0)
			{
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			// Texture
			else if (strcmp(lineHeader, "vt") == 0)
			{
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
			}
			// Normals
			else if (strcmp(lineHeader, "vn") == 0)
			{
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			// Faces
			else if (strcmp(lineHeader, "f") == 0)
			{
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				ASSERT(matches == 9, "File can't be read!")

				temp_vertexIndices.push_back(vertexIndex[0]);
				temp_vertexIndices.push_back(vertexIndex[1]);
				temp_vertexIndices.push_back(vertexIndex[2]);
				temp_uvIndices.push_back(uvIndex[0]);
				temp_uvIndices.push_back(uvIndex[1]);
				temp_uvIndices.push_back(uvIndex[2]);
				temp_normalIndices.push_back(normalIndex[0]);
				temp_normalIndices.push_back(normalIndex[1]);
				temp_normalIndices.push_back(normalIndex[2]);
			}
		}

		for (unsigned int i = 0; i < temp_vertexIndices.size(); i++) {
			unsigned int vertexIndex = temp_vertexIndices[i];
			unsigned int uvIndex = temp_uvIndices[i];
			unsigned int normalIndex = temp_normalIndices[i];

			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			glm::vec3 normal = temp_normals[normalIndex - 1];


			out_vertices.push_back(vertex.x);
			out_vertices.push_back(vertex.y);
			out_vertices.push_back(vertex.z);
			out_vertices.push_back(uv.x);
			out_vertices.push_back(uv.y);
			out_vertices.push_back(normal.x);
			out_vertices.push_back(normal.y);
			out_vertices.push_back(normal.z);

			out_indices.push_back(i);
		}
		LOG_TRACE("File loaded successfully!");
		fclose(file);
		return true;
	}

	Ref<Mesh> LoadMesh(const std::string& name, const std::string& path)
	{
		std::vector<float> vertices;
		std::vector<uint32_t> indices;

		loadOBJ(path.c_str(), vertices, indices);

		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
		vb->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normals" }
		});

		Ref<IndexBuffer> ib = IndexBuffer::Create(indices.data(), indices.size());

		Ref<Mesh> mesh = CreateRef<Mesh>(name, vb);
		mesh->AddSubmesh(ib);

		return mesh;
	}

}

