#include "egpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include "glm/gtc/type_ptr.hpp"

namespace Engine {

	Ref<UniformBuffer> Renderer::s_SceneUB;

	void Renderer::Init()
	{
		OpenGL::API::Init();
		s_SceneUB = UniformBuffer::Create({
			{ShaderDataType::Mat4, "ViewProjection"}
		});
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
		glm::mat4 viewProjectionMatrix = camera->GetProjection() * glm::inverse(transform);
		s_SceneUB->SetData(glm::value_ptr(viewProjectionMatrix), "ViewProjection");

		// lights...
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();

		// once per shader
		shader->SetBlockBinding("SceneData", 0);
		s_SceneUB->Bind(0);

		// once per object
		shader->SetMat4("u_Transform", transform);
		glm::mat4 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform)));
		shader->SetMat3("u_NormalMatrix", normalMatrix);

		// Material System
		// material->Bind(s_SceneUniforms);
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
		// texture->Bind(0);

		// ...
	}

	void Renderer::Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		// once per material
		auto& shader = material->GetShader();
		auto& materialUB = material->GetUniformBuffer();

		shader->Bind();

		shader->SetBlockBinding("SceneData", 0);
		s_SceneUB->Bind(0);
		shader->SetBlockBinding("MaterialData", 1);
		materialUB->Bind(1);

		// once per object
		shader->SetMat4("u_Transform", transform);
		glm::mat4 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform)));
		shader->SetMat3("u_NormalMatrix", normalMatrix);

		vertexArray->Bind();
		OpenGL::API::DrawIndexed(vertexArray, vertexArray->GetIndexBuffer()->GetCount());
	}

}