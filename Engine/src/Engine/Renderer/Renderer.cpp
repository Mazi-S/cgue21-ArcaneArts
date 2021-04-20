#include "egpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Engine {

	Ref<OpenGL::GlUniformBuffer> Renderer::s_SceneUB;
	Ref<OpenGL::GlFramebuffer> Renderer::s_ShadowMapFB;
	Ref<OpenGL::GlShader> Renderer::s_ShadowMapShader;

	SceneData Renderer::s_SceneData;

	std::map<Material*, std::set<RenderableObject, RenderableObject>> Renderer::s_RenderQueue;
	std::map<OpenGL::GlVertexArray*, std::set<RenderableObject, RenderableObject>> Renderer::s_DepthQueue;
	std::set<Ref<Material>> Renderer::s_Materials;
	std::set<Ref<OpenGL::GlVertexArray>> Renderer::s_VertexArrays;

	void Renderer::Init()
	{
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
			{OpenGL::GlShaderDataType::Float, "PointLight_Quadratic", 4 * 4 * 9 + 4 * 1}
		});
		s_SceneUB = CreateRef<OpenGL::GlUniformBuffer>(layout);

		// Shadow Map
		s_ShadowMapShader = CreateRef<OpenGL::GlShader>("ShadowMapShader", "assets/shaders/ShadowMap.glsl");

		OpenGL::FramebufferSpecification spec;
		spec.Height = 1024 * 5;
		spec.Width  = 1024 * 5;
		spec.DepthAttachment.Attach = true;
		spec.DepthAttachment.Internalformat = GL_DEPTH_COMPONENT;
		spec.DepthAttachment.Format = GL_DEPTH_COMPONENT;
		spec.DepthAttachment.Type = GL_FLOAT;
		spec.DepthAttachment.Wrap_S = GL_CLAMP_TO_BORDER;
		spec.DepthAttachment.Wrap_T = GL_CLAMP_TO_BORDER;
		spec.DepthAttachment.Border = { 1.0f, 1.0f, 1.0f, 1.0f };
		spec.DepthAttachment.Min_Filter = GL_LINEAR;
		spec.DepthAttachment.Mag_Filter = GL_LINEAR;
		spec.DepthAttachment.ComparisonMode = GL_COMPARE_REF_TO_TEXTURE;

		s_ShadowMapFB = CreateRef<OpenGL::GlFramebuffer>(spec);
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
		// SceneData
		s_SceneData.Camera = camera;
		s_SceneData.CameraPos = { s_SceneData.Camera.Transform[3][0], s_SceneData.Camera.Transform[3][1], s_SceneData.Camera.Transform[3][2] };

		s_SceneData.DirectionalLight = directionalLight;
		s_SceneData.PointLight = pointLight;
		s_SceneData.DepthProjectionMatrix = glm::ortho<float>(-50, 50, -50, 50, -50, 50);
		s_SceneData.DepthViewMatrix = glm::lookAt(s_SceneData.CameraPos - directionalLight.Direction, s_SceneData.CameraPos, glm::vec3(0, 1, 0));
	}

	void Renderer::EndScene()
	{
		UpdateShadowMap();
		Render();

		s_Materials.clear();
		s_VertexArrays.clear();
		s_RenderQueue.clear();
		s_DepthQueue.clear();
	}

	void Renderer::Submit(const Ref<OpenGL::GlVertexArray>& vertexArray, const Ref<Material>& material, const glm::mat4& transform, bool shadow)
	{
		s_Materials.insert(material);
		s_VertexArrays.insert(vertexArray);
		s_RenderQueue[material.get()].insert(RenderableObject(material.get(), vertexArray.get(), transform));

		if (shadow)
			s_DepthQueue[vertexArray.get()].insert(RenderableObject(material.get(), vertexArray.get(), transform));
	}

	void Renderer::Submit(const Ref<OpenGL::GlMesh>& mesh, const Ref<Material>& material, const glm::mat4& transform, bool shadow)
	{
		for(auto& submesh : mesh->GetSubmeshes())
			Submit(submesh->GetVertexArray(), material, transform, shadow);
	}

	void Renderer::Render()
	{
		// Update SceneUB
		glm::mat4 viewProjectionMatrix = s_SceneData.Camera.Projection * glm::inverse(s_SceneData.Camera.Transform);
		s_SceneUB->SetData(glm::value_ptr(viewProjectionMatrix), "ViewProjection");
		s_SceneUB->SetData(glm::value_ptr(s_SceneData.CameraPos), "CameraPosition");

		// Lights
		s_SceneUB->SetData(glm::value_ptr(s_SceneData.DirectionalLight.Direction), "DirectionalLight_Direction");
		s_SceneUB->SetData(glm::value_ptr(s_SceneData.DirectionalLight.Color), "DirectionalLight_Color");

		s_SceneUB->SetData(glm::value_ptr(s_SceneData.PointLight.Position), "PointLight_Position");
		s_SceneUB->SetData(glm::value_ptr(s_SceneData.PointLight.Color), "PointLight_Color");
		s_SceneUB->SetData(&s_SceneData.PointLight.Constant, "PointLight_Constant");
		s_SceneUB->SetData(&s_SceneData.PointLight.Linear, "PointLight_Linear");
		s_SceneUB->SetData(&s_SceneData.PointLight.Quadratic, "PointLight_Quadratic");

		glm::mat4 lightSpaceMatrix = s_SceneData.DepthProjectionMatrix * s_SceneData.DepthViewMatrix;
		for (const auto& material : s_RenderQueue)
		{
			material.first->Bind();
			material.first->Set("SceneData", s_SceneUB);

			auto& shader = material.first->GetShader();
			s_ShadowMapFB->GetDepthAttachment()->Bind(2);
			shader->SetInt("u_ShadowMap", 2);

			for (const auto& obj : material.second)
			{
				shader->SetMat4("u_Transform", obj.Transform);
				glm::mat4 normalMatrix = glm::mat3(glm::transpose(glm::inverse(obj.Transform)));
				shader->SetMat3("u_NormalMatrix", normalMatrix);

				shader->SetMat4("u_LightSpaceMatrix", lightSpaceMatrix * obj.Transform);

				obj.VertexArray->Bind();
				OpenGL::API::DrawIndexed(obj.VertexArray, obj.VertexArray->GetIndexBuffer()->GetCount());
			}
		}
	}

	void Renderer::UpdateShadowMap()
	{
		glm::mat4 depthVP = s_SceneData.DepthProjectionMatrix * s_SceneData.DepthViewMatrix;

		s_ShadowMapFB->Bind();
		OpenGL::API::Clear();
		OpenGL::API::CullFaces(OpenGL::Face::Front);

		s_ShadowMapShader->Bind();
		for (const auto& vertexArray : s_DepthQueue)
		{
			vertexArray.first->Bind();
			for (const auto& obj : vertexArray.second)
			{
				s_ShadowMapShader->SetMat4("u_DepthMVP", depthVP * obj.Transform);
				OpenGL::API::DrawIndexed(obj.VertexArray, obj.VertexArray->GetIndexBuffer()->GetCount());
			}
		}
		s_ShadowMapFB->Unbind();

		OpenGL::API::CullFaces();
		OpenGL::API::SetViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	}

}