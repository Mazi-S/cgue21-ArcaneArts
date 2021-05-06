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

	void ObjectLoader::LoadMesh(const std::string& name, const std::string& path, std::vector<glm::vec3>& positions_out, std::vector<glm::vec3>& normals_out, std::vector<glm::vec2>& textureCoordinates_out, std::vector<Submesh>& submeshes_out)
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

}

