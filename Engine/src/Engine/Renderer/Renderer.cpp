#include "egpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLAPI.h"

namespace Engine {

	Renderer::SceneData Renderer::s_SceneData;

	void Renderer::Init()
	{
		OpenGL::API::Init();
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		OpenGL::API::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Ref<Camera>& camera, const glm::mat4& transform)
	{
		s_SceneData.ViewProjectionMatrix = camera->GetProjection() * glm::inverse(transform);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData.ViewProjectionMatrix);	// once per shader
		shader->SetMat4("u_Transform", transform);	// once per object

		// Material System
		// material->Bind();
			// Todo: remove (just temporary)
			shader->SetFloat4("u_Color", glm::vec4(0.8f, 0.15f, 0.2f, 1.0f));

		vertexArray->Bind();
		OpenGL::API::DrawIndexed(vertexArray, vertexArray->GetIndexBuffer()->GetCount());
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const Ref<Texture>& texture, const glm::mat4& transform)
	{
		shader->Bind();
		// ...

		// Bind texture at slot 0
		texture->Bind(0);

		// ...
	}

}