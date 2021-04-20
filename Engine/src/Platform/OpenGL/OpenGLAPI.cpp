#include "egpch.h"
#include "OpenGLAPI.h"

#include <glad/glad.h>

namespace Engine::OpenGL {

	void API::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	void API::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void API::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void API::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void API::CullFaces(Face face)
	{
		glCullFace(uint32_t(face));
	}

	void API::DepthFunc(DepthFunction func)
	{
		glDepthFunc(uint32_t(func));
	}

	void API::DrawIndexed(const Ref<OpenGL::GlVertexArray>& vertexArray, uint32_t indexCount)
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}

	void API::DrawIndexed(const OpenGL::GlVertexArray* vertexArray, uint32_t indexCount)
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}

	void API::UnbindVertexArray()
	{
		glBindVertexArray(0);
	}

}
