#include "egpch.h"
#include "ShadowMap.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

	Ref<OpenGL::GlUniformBuffer> ShadowMap::s_UniformBuffer;
	Ref<OpenGL::GlFramebuffer> ShadowMap::s_Framebuffer;
	Ref<OpenGL::GlShader> ShadowMap::s_Shader;

	std::map<OpenGL::GlVertexArray*, std::set<ShadableObject, ShadableObject>> ShadowMap::s_DepthQueue;
	std::set<Ref<OpenGL::GlVertexArray>> ShadowMap::s_VertexArrays;

	bool ShadableObject::operator()(const ShadableObject& left, const ShadableObject& right) const
	{
		if (left.VertexArray < right.VertexArray)
			return true;
		if (left.VertexArray > right.VertexArray)
			return false;

		auto temp = right.Transform - left.Transform;

		if (temp[3][0] < 0)
			return true;
		if (temp[3][0] > 0)
			return false;

		if (temp[3][1] < 0)
			return true;
		if (temp[3][1] > 0)
			return false;

		if (temp[3][2] < 0)
			return true;
		if (temp[3][2] > 0)
			return false;

		if (temp[3][3] < 0)
			return true;
		if (temp[3][3] > 0)
			return false;

		if (temp[2][0] < 0)
			return true;
		if (temp[2][0] > 0)
			return false;

		if (temp[2][1] < 0)
			return true;
		if (temp[2][1] > 0)
			return false;

		if (temp[2][2] < 0)
			return true;
		if (temp[2][2] > 0)
			return false;

		if (temp[2][3] < 0)
			return true;
		if (temp[2][3] > 0)
			return false;

		if (temp[1][0] < 0)
			return true;
		if (temp[1][0] > 0)
			return false;

		if (temp[1][1] < 0)
			return true;
		if (temp[1][1] > 0)
			return false;

		if (temp[1][2] < 0)
			return true;
		if (temp[1][2] > 0)
			return false;

		if (temp[1][3] < 0)
			return true;
		if (temp[1][3] > 0)
			return false;

		if (temp[0][0] < 0)
			return true;
		if (temp[0][0] > 0)
			return false;

		if (temp[0][1] < 0)
			return true;
		if (temp[0][1] > 0)
			return false;

		if (temp[0][2] < 0)
			return true;
		if (temp[0][2] > 0)
			return false;

		if (temp[0][3] < 0)
			return true;
		if (temp[0][3] > 0)
			return false;

		return false;
	}

	void ShadowMap::Init()
	{
		LOG_INFO("ShadowMap::Init...");

		s_Shader = CreateRef<OpenGL::GlShader>("ShadowMapShader", "assets/shaders/ShadowMap.glsl");

		OpenGL::GlUniformBufferLayout_std140 layout(4 * 4 * 4, {
			{OpenGL::GlShaderDataType::Mat4, "LightSpaceMatrix", 0}
		});

		s_UniformBuffer = CreateRef<OpenGL::GlUniformBuffer>(layout);
		s_UniformBuffer->Bind(6);

		OpenGL::FramebufferSpecification spec;
		spec.Height = 1024 * 5;
		spec.Width = 1024 * 5;
		spec.DepthAttachment.Attach = true;
		spec.DepthAttachment.Internalformat = 0x1902;
		spec.DepthAttachment.Format = 0x1902;
		spec.DepthAttachment.Type = 0x1406;
		spec.DepthAttachment.Wrap_S = 0x812D;
		spec.DepthAttachment.Wrap_T = 0x812D;
		spec.DepthAttachment.Border = { 1.0f, 1.0f, 1.0f, 1.0f };
		spec.DepthAttachment.Min_Filter = 0x2601;
		spec.DepthAttachment.Mag_Filter = 0x2601;
		spec.DepthAttachment.ComparisonMode = 0x884E;
		spec.DepthAttachment.TextureLibrary = true;
		spec.DepthAttachment.Name = "ShadowMap";

		s_Framebuffer = CreateRef<OpenGL::GlFramebuffer>(spec);
	}

	void ShadowMap::BeginScene(const Camera& camera, const DirectionalLight& directionalLight)
	{
		auto depthProjectionMatrix = glm::ortho<float>(-50, 50, -50, 50, -50, 50);
		auto depthViewMatrix = glm::lookAt(camera.Position() - directionalLight.Direction, camera.Position(), glm::vec3(0, 1, 0));

		glm::mat4 lightSpaceMatrix = depthProjectionMatrix * depthViewMatrix;
		s_UniformBuffer->SetData(glm::value_ptr(lightSpaceMatrix), "LightSpaceMatrix");
	}

	void ShadowMap::EndScene()
	{
		UpdateShadowMap();

		s_VertexArrays.clear();
		s_DepthQueue.clear();
	}

	void ShadowMap::Submit(const Ref<OpenGL::GlVertexArray>& vertexArray, const glm::mat4& transform)
	{
		s_VertexArrays.insert(vertexArray);
		s_DepthQueue[vertexArray.get()].insert(ShadableObject(vertexArray.get(), transform));
	}

	void ShadowMap::Submit(const Ref<OpenGL::GlMesh>& mesh, const glm::mat4& transform)
	{
		for (auto& submesh : mesh->GetSubmeshes())
			Submit(submesh->GetVertexArray(), transform);
	}

	void ShadowMap::UpdateShadowMap()
	{
		s_Framebuffer->Bind();
		OpenGL::API::Clear();
		OpenGL::API::CullFaces(OpenGL::Face::Front);

		s_Shader->Bind();
		for (const auto& vertexArray : s_DepthQueue)
		{
			vertexArray.first->Bind();
			for (const auto& obj : vertexArray.second)
			{
				OpenGL::API::DrawIndexed(obj.VertexArray, obj.VertexArray->GetIndexBuffer()->GetCount());
			}
		}
		s_Framebuffer->Unbind();

		OpenGL::API::CullFaces();
		OpenGL::API::SetViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	}

}