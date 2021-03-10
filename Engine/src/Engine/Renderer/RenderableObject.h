#pragma once
#include "Material.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine {

	struct RenderableObject
	{
		const Material* Material = nullptr;
		const OpenGL::GlVertexArray* VertexArray = nullptr;
		const glm::mat4 Transform = glm::mat4(1);

		RenderableObject() = default;

		RenderableObject(Engine::Material* material,
			OpenGL::GlVertexArray* vertexArray,
			const glm::mat4& transform)
			: Material(material), VertexArray(vertexArray), Transform(transform) { }

		bool operator()(const RenderableObject& left, const RenderableObject& right) const;

	};

}