#include "egpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

	Ref<OpenGL::GlUniformBuffer> Renderer::s_SceneUB;

	void Renderer::Init()
	{
		OpenGL::API::Init();
		OpenGL::GlUniformBufferLayout_std140 layout(4 * 4 * 10, {
			{OpenGL::GlShaderDataType::Mat4, "ViewProjection", 0},
			{OpenGL::GlShaderDataType::Float3, "CameraPosition", 4 * 4 * 4},
			{OpenGL::GlShaderDataType::Float3, "DirectionalLight_Direction", 4 * 4 * 5},
			{OpenGL::GlShaderDataType::Float3, "DirectionalLight_Color", 4 * 4 * 6},
			{OpenGL::GlShaderDataType::Float3, "PointLight_Position", 4 * 4 * 7},
			{OpenGL::GlShaderDataType::Float3, "PointLight_Color", 4 * 4 * 8},
			{OpenGL::GlShaderDataType::Float, "PointLight_Constant", 4 * 4 * 8 + 4 * 3},
			{OpenGL::GlShaderDataType::Float, "PointLight_Linear", 4 * 4 * 9},
			{OpenGL::GlShaderDataType::Float, "PointLight_Quadratic", 4 * 4 * 9 + 4 * 1}
		});
		s_SceneUB = CreateRef<OpenGL::GlUniformBuffer>(layout);
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		OpenGL::API::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Camera& camera, const DirectionalLight& directionalLight, const PointLight& pointLight)
	{
		glm::mat4 viewProjectionMatrix = camera.Projection * glm::inverse(camera.Transform);
		s_SceneUB->SetData(glm::value_ptr(viewProjectionMatrix), "ViewProjection");

		glm::vec3 cameraPos = { camera.Transform[3][0], camera.Transform[3][1], camera.Transform[3][2] };
		s_SceneUB->SetData(glm::value_ptr(cameraPos), "CameraPosition");

		// Lights
		s_SceneUB->SetData(glm::value_ptr(directionalLight.Direction), "DirectionalLight_Direction");
		s_SceneUB->SetData(glm::value_ptr(directionalLight.Color), "DirectionalLight_Color");

		s_SceneUB->SetData(glm::value_ptr(pointLight.Position), "PointLight_Position");
		s_SceneUB->SetData(glm::value_ptr(pointLight.Color), "PointLight_Color");
		s_SceneUB->SetData(&pointLight.Constant, "PointLight_Constant");
		s_SceneUB->SetData(&pointLight.Linear, "PointLight_Linear");
		s_SceneUB->SetData(&pointLight.Quadratic, "PointLight_Quadratic");
	}

	void Renderer::Submit(const Ref<OpenGL::GlVertexArray>& vertexArray, const Ref<Material>& material, const glm::mat4& transform)
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
		for(auto& submesh : mesh->GetGlMesh()->GetSubmeshes())
			Submit(submesh->GetVertexArray(), material, transform);
	}

}