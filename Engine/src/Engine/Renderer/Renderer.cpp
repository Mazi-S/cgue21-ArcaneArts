#include "egpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/OpenGL/OpenGLUtil.h"
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
		OpenGL::GlUniformBufferLayout_std140 layout(4 * 4 * 22, {
			{OpenGL::GlShaderDataType::Mat4, "ViewProjection", 0},
			{OpenGL::GlShaderDataType::Float3, "CameraPosition", 4 * 4 * 4},

			{OpenGL::GlShaderDataType::Float3,	"DirectionalLight_Direction", 4 * 4 * 5},
			{OpenGL::GlShaderDataType::Float3,	"DirectionalLight_Color", 4 * 4 * 6},

			{OpenGL::GlShaderDataType::Int,		"PointLightCount", 4 * 4 * 6 + 4 * 3},

			{OpenGL::GlShaderDataType::Struct,	"PointLight0", 4 * 4 * 7},
			{OpenGL::GlShaderDataType::Struct,	"PointLight1", 4 * 4 * 10},
			{OpenGL::GlShaderDataType::Struct,	"PointLight2", 4 * 4 * 13},
			{OpenGL::GlShaderDataType::Struct,	"PointLight3", 4 * 4 * 16},
			{OpenGL::GlShaderDataType::Struct,	"PointLight4", 4 * 4 * 19},
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

	void Renderer::BeginScene(const Camera& camera, const DirectionalLight& directionalLight, const std::vector<PointLight>& pointLights)
	{
		// Update SceneUB
		glm::mat4 viewProjectionMatrix = camera.ViewProjection();
		s_SceneUB->SetData(glm::value_ptr(viewProjectionMatrix), "ViewProjection");
		s_SceneUB->SetData(glm::value_ptr(camera.Position()), "CameraPosition");

		// Lights
		s_SceneUB->SetData(glm::value_ptr(directionalLight.Direction), "DirectionalLight_Direction");
		s_SceneUB->SetData(glm::value_ptr(directionalLight.Color), "DirectionalLight_Color");

		int pointLightCount = std::min(5, (int)pointLights.size());
		s_SceneUB->SetData(&pointLightCount, "PointLightCount");

		const static std::string bufferElementName[] = { "PointLight0", "PointLight1", "PointLight2", "PointLight3", "PointLight4" };
		const static int offsetPosition = 0;
		const static int offsetColor = 4 * 4 * 1;
		const static int offsetConstant = 4 * 4 * 1 + 4 * 3;
		const static int offsetLinear = 4 * 4 * 2;
		const static int offsetQuadratic = 4 * 4 * 2 + 4;

		for (size_t i = 0; i < pointLightCount; i++)
		{
			const PointLight& pointLight = pointLights[i];
			const OpenGL::GlBufferElement& bufferElement = s_SceneUB->GetElement(bufferElementName[i]);

			s_SceneUB->SetData(glm::value_ptr(pointLight.Position),		OpenGL::Util::ShaderDataTypeSize(OpenGL::GlShaderDataType::Float3),		bufferElement.Offset + offsetPosition);
			s_SceneUB->SetData(glm::value_ptr(pointLight.Color),		OpenGL::Util::ShaderDataTypeSize(OpenGL::GlShaderDataType::Float3),		bufferElement.Offset + offsetColor);
			s_SceneUB->SetData(&pointLight.Constant,					OpenGL::Util::ShaderDataTypeSize(OpenGL::GlShaderDataType::Float),		bufferElement.Offset + offsetConstant);
			s_SceneUB->SetData(&pointLight.Linear,						OpenGL::Util::ShaderDataTypeSize(OpenGL::GlShaderDataType::Float),		bufferElement.Offset + offsetLinear);
			s_SceneUB->SetData(&pointLight.Quadratic,					OpenGL::Util::ShaderDataTypeSize(OpenGL::GlShaderDataType::Float),		bufferElement.Offset + offsetQuadratic);
		}


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

	void Renderer::Submit(const std::string& meshName, const std::vector<std::string>& materials, const glm::mat4& transform)
	{
		Ref<OpenGL::GlMesh>& mesh = MeshLibrary::Get(meshName)->GetGlMesh();
		auto& submeshes = mesh->GetSubmeshes();

		for (size_t i = 0; i < submeshes.size(); i++)
		{
			Ref<Material>& material = i < materials.size() ? MaterialLibrary::Get(materials[i]) : MaterialLibrary::Get();
			Submit(submeshes[i]->GetVertexArray(), material, transform);
		}
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