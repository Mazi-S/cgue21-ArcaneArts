#include "egpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Engine {

	Ref<OpenGL::GlUniformBuffer> Renderer::s_SceneUB;

	std::map<Material*, std::set<RenderableObject, RenderableObject>> Renderer::s_RenderQueue;
	std::set<Ref<Material>> Renderer::s_Materials;
	std::set<Ref<OpenGL::GlVertexArray>> Renderer::s_VertexArrays;

	void Renderer::Init()
	{
		LOG_INFO("Renderer::Init...");
		OpenGL::API::Init();
		OpenGL::GlUniformBufferLayout_std140 layout(4 * 4 * 14, {
			{OpenGL::GlShaderDataType::Mat4, "ViewProjection", 0},
			{OpenGL::GlShaderDataType::Float3, "CameraPosition", 4 * 4 * 4},
			{OpenGL::GlShaderDataType::Float3, "DirectionalLight_Direction", 4 * 4 * 5},
			{OpenGL::GlShaderDataType::Float3, "DirectionalLight_Color", 4 * 4 * 6},
			{OpenGL::GlShaderDataType::Float3, "PointLight_Position", 4 * 4 * 7},
			{OpenGL::GlShaderDataType::Float3, "PointLight_Color", 4 * 4 * 8},
			{OpenGL::GlShaderDataType::Float, "PointLight_Constant", 4 * 4 * 8 + 4 * 3},
			{OpenGL::GlShaderDataType::Float, "PointLight_Linear", 4 * 4 * 9},
			{OpenGL::GlShaderDataType::Float, "PointLight_Quadratic", 4 * 4 * 9 + 4 * 1},
			{OpenGL::GlShaderDataType::Mat4, "PointLight_Quadratic", 4 * 4 * 9 + 4 * 1},
			{OpenGL::GlShaderDataType::Float, "PointLight_Quadratic", 4 * 4 * 9 + 4 * 1},
		});
		s_SceneUB = CreateRef<OpenGL::GlUniformBuffer>(layout);
		s_SceneUB->Bind(0);
	}

	void Renderer::Shutdown()
	{
		LOG_INFO("Renderer2D::Shutdown...");
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		OpenGL::API::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Camera& camera, const DirectionalLight& directionalLight, const PointLight& pointLight)
	{
		// Update SceneUB
		glm::mat4 viewProjectionMatrix = camera.ViewProjection();
		s_SceneUB->SetData(glm::value_ptr(viewProjectionMatrix), "ViewProjection");
		s_SceneUB->SetData(glm::value_ptr(camera.Position()), "CameraPosition");

		// Lights
		s_SceneUB->SetData(glm::value_ptr(directionalLight.Direction), "DirectionalLight_Direction");
		s_SceneUB->SetData(glm::value_ptr(directionalLight.Color), "DirectionalLight_Color");

		s_SceneUB->SetData(glm::value_ptr(pointLight.Position), "PointLight_Position");
		s_SceneUB->SetData(glm::value_ptr(pointLight.Color), "PointLight_Color");
		s_SceneUB->SetData(&pointLight.Constant, "PointLight_Constant");
		s_SceneUB->SetData(&pointLight.Linear, "PointLight_Linear");
		s_SceneUB->SetData(&pointLight.Quadratic, "PointLight_Quadratic");
	}

	void Renderer::EndScene()
	{
		Render();

		s_Materials.clear();
		s_VertexArrays.clear();
		s_RenderQueue.clear();
	}

	void Renderer::Submit(const Ref<OpenGL::GlVertexArray>& vertexArray, const Ref<Material>& material, const glm::mat4& transform)
	{
		s_Materials.insert(material);
		s_VertexArrays.insert(vertexArray);
		s_RenderQueue[material.get()].insert(RenderableObject(material.get(), vertexArray.get(), transform));
	}

	void Renderer::Submit(const std::string& meshName, const std::string& materialName, const glm::mat4& transform)
	{
		Ref<OpenGL::GlMesh>& mesh = MeshLibrary::Get(meshName)->GetGlMesh();
		Ref<Material>& material = MaterialLibrary::Get(materialName);

		for(auto& submesh : mesh->GetSubmeshes())
			Submit(submesh->GetVertexArray(), material, transform);
	}

	void Renderer::Render()
	{
		for (const auto& material : s_RenderQueue)
		{
			material.first->Bind();

			Ref<OpenGL::GlShader>& shader = ShaderLibrary::Get(material.first->GetShader());

			for (const auto& obj : material.second)
			{
				shader->SetMat4("u_Transform", obj.Transform);
				glm::mat4 normalMatrix = glm::mat3(glm::transpose(glm::inverse(obj.Transform)));
				shader->SetMat3("u_NormalMatrix", normalMatrix);

				obj.VertexArray->Bind();
				OpenGL::API::DrawIndexed(obj.VertexArray, obj.VertexArray->GetIndexBuffer()->GetCount());
			}
		}
		OpenGL::API::UnbindVertexArray();
	}
}