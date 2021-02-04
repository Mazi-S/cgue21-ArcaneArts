#include "egpch.h"

#include <vector>
#include <set>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include "ObjectLoader.h"
#include "tiny_obj_loader.h"

namespace Engine {

	bool ObjectLoader::loadOBJ(
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

	Ref<Mesh> ObjectLoader::LoadMesh(const std::string& name, const std::string& path)
	{
		std::set<Ref<IndexBuffer>> indexBuffers;

		std::vector<float> vertices;
		std::vector<uint32_t> indices;

		tinyobj::ObjReaderConfig reader_config;
		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(path, reader_config))
		{
			if (!reader.Error().empty())
				LOG_ERROR("TinyObjReader: {}", reader.Error());
			ASSERT(false, "Failed to load Mesh!");
		}

		if (!reader.Warning().empty())
			LOG_WARN("ObjectLoader: {}", reader.Warning());

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();

		// Loop over shapes
		uint32_t indexCount = 0;
		for (size_t s = 0; s < shapes.size(); s++)
		{
			LOG_INFO("path: {}\t shapes: {}", path, shapes.size());
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				int fv = shapes[s].mesh.num_face_vertices[f];
				ASSERT(fv == 3, "ObjectLoader: Invalid number of vertices per face!");

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++)
				{
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
					tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];

					vertices.push_back(vx);
					vertices.push_back(vy);
					vertices.push_back(vz);
					vertices.push_back(tx);
					vertices.push_back(ty);
					vertices.push_back(nx);
					vertices.push_back(ny);
					vertices.push_back(nz);

					indices.push_back(indexCount++);
				}
				index_offset += fv;

			}

			auto ib = IndexBuffer::Create(indices.data(), indices.size());
			indices.clear();
			indexBuffers.insert(ib);
		}

		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
		vb->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normals" }
		});

		Ref<Mesh> mesh = CreateRef<Mesh>(name, vb);
		for (auto& ib : indexBuffers)
		{
			mesh->AddSubmesh(ib);
		}

		return mesh;
	}

}

