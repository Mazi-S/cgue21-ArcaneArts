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

	void ObjectLoader::LoadMesh(const std::string& path, std::vector<glm::vec3>& positions_out, std::vector<glm::vec3>& normals_out, std::vector<glm::vec2>& textureCoordinates_out, std::vector<Submesh>& submeshes_out)
	{
		std::set<std::vector<uint32_t>> submeshIndices;

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

		for (size_t i = 0; i < attrib.vertices.size(); i += 3)
		{
			positions_out.push_back({
				attrib.vertices[i + 0],
				attrib.vertices[i + 1],
				attrib.vertices[i + 2]
				});
		}

		for (size_t i = 0; i < attrib.texcoords.size(); i += 2)
		{
			textureCoordinates_out.push_back({
				attrib.texcoords[i + 0],
				attrib.texcoords[i + 1],
				});
		}

		for (size_t i = 0; i < attrib.normals.size(); i += 3)
		{
			normals_out.push_back({
				attrib.normals[i + 0],
				attrib.normals[i + 1],
				attrib.normals[i + 2]
				});
		}

		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++)
		{
			Submesh submesh;

			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				Face face;
				face.vertices = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face.
				for (size_t v = 0; v < face.vertices; v++)
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					face.positionIndex.push_back(idx.vertex_index);
					face.textureCoordinateIndex.push_back(idx.texcoord_index);
					face.normalIndex.push_back(idx.normal_index);
				}
				index_offset += face.vertices;
				submesh.Faces.push_back(face);
			}
			submeshes_out.push_back(submesh);
		}
	}

	void ObjectLoader::LoadMesh(const std::string& path, std::vector<glm::vec3>& positions_out, std::vector<glm::vec3>& normals_out, std::vector<glm::vec2>& textureCoordinates_out, std::vector<glm::vec3>& tangent_out, std::vector<glm::vec3>& bitangent_out, std::vector<Submesh>& submeshes_out)
	{
		LoadMesh(path, positions_out, normals_out, textureCoordinates_out, submeshes_out);
		
		for (Submesh& submesh : submeshes_out)
		{
			for (Face& face : submesh.Faces)
			{
				ASSERT(face.vertices == 3, "Invalid count of vertices!");

				glm::vec3 pos1 = positions_out[face.positionIndex[0]];
				glm::vec3 pos2 = positions_out[face.positionIndex[1]];
				glm::vec3 pos3 = positions_out[face.positionIndex[2]];
				glm::vec2 textureCoord1 = textureCoordinates_out[face.textureCoordinateIndex[0]];
				glm::vec2 textureCoord2 = textureCoordinates_out[face.textureCoordinateIndex[1]];
				glm::vec2 textureCoord3 = textureCoordinates_out[face.textureCoordinateIndex[2]];

				glm::vec3 v = pos2 - pos1;
				glm::vec3 w = pos3 - pos1;

				glm::vec2 deltaTextureCoord1 = textureCoord2 - textureCoord1;
				glm::vec2 deltaTextureCoord2 = textureCoord3 - textureCoord1;
				
				float dirCorrection = (deltaTextureCoord2.x * deltaTextureCoord1.y - deltaTextureCoord2.y * deltaTextureCoord1.x) < 0.0f ? -1.0f : 1.0f;

				if (deltaTextureCoord1.x * deltaTextureCoord2.y == deltaTextureCoord1.y * deltaTextureCoord2.x)
				{
					deltaTextureCoord1 = { 0, 1 };
					deltaTextureCoord2 = { 1, 0 };
				}

				glm::vec3 tangent;
				glm::vec3 bitangent;
				tangent.x = (w.x * deltaTextureCoord1.y - v.x * deltaTextureCoord2.y) * dirCorrection;
				tangent.y = (w.y * deltaTextureCoord1.y - v.y * deltaTextureCoord2.y) * dirCorrection;
				tangent.z = (w.z * deltaTextureCoord1.y - v.z * deltaTextureCoord2.y) * dirCorrection;
				bitangent.x = (w.x * deltaTextureCoord1.x - v.x * deltaTextureCoord2.x) * dirCorrection;
				bitangent.y = (w.y * deltaTextureCoord1.x - v.y * deltaTextureCoord2.x) * dirCorrection;
				bitangent.z = (w.z * deltaTextureCoord1.x - v.z * deltaTextureCoord2.x) * dirCorrection;

				for (unsigned int i = 0; i < 3; i++)
				{
					glm::vec3 normal = normals_out[face.normalIndex[i]];

					glm::vec3 localTangent   = glm::normalize(tangent - normal * (tangent * normal));
					glm::vec3 localBitangent = glm::normalize(bitangent - normal * (bitangent * normal));

					face.tangentIndex.push_back(tangent_out.size());

					tangent_out.push_back(localTangent);
					bitangent_out.push_back(localBitangent);
				}
			}
		}
	}

}

