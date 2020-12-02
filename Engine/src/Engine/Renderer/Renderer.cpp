#include "egpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include "glm/gtc/type_ptr.hpp"

namespace Engine {

	Ref<UniformBuffer> Renderer::s_SceneUB;

	void Renderer::Init()
	{
		OpenGL::API::Init();
		s_SceneUB = UniformBuffer::Create(4 * 4 * 4, {
			{ShaderDataType::Mat4, "ViewProjection", 0}
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

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Material>& material, const glm::mat4& transform)
	{
		// once per material
		material->Bind();
		material->Set("SceneData", s_SceneUB);

		auto& shader = material->GetShader();

		// once per object
		shader->SetMat4("u_Transform", transform);
		glm::mat4 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform)));
		shader->SetMat3("u_NormalMatrix", normalMatrix);

		vertexArray->Bind();
		OpenGL::API::DrawIndexed(vertexArray, vertexArray->GetIndexBuffer()->GetCount());
	}

	void Renderer::Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const glm::mat4& transform)
	{
		for(auto& submesh : mesh->GetSubmeshes())
			Submit(submesh->GetVertexArray(), material, transform);
	}

}