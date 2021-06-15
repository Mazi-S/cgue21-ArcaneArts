#include "egpch.h"
#include "Renderer.h"

#include "Platform/Platform.h"
#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/OpenGL/OpenGLUtil.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Engine {
	Ref<OpenGL::GlUniformBuffer> Renderer::s_SceneUB;
	Ref<OpenGL::GlUniformBuffer> Renderer::s_ModelUB;

	float Renderer::s_Exposure = 1.0f;
	bool Renderer::s_HDR = true;
	bool Renderer::s_Bloom = true;
	bool Renderer::s_GammaCorrection = false;

	Ref<OpenGL::GlFramebuffer> Renderer::s_HDRFramebuffer;
	Ref<OpenGL::GlFramebuffer> Renderer::s_PingPongFramebuffer0;
	Ref<OpenGL::GlFramebuffer> Renderer::s_PingPongFramebuffer1;
	Scope<OpenGL::GlShader> Renderer::s_HDRShader;
	Scope<OpenGL::GlShader> Renderer::s_GaussianBlurShader;
	Scope<OpenGL::GlVertexArray> Renderer::s_QuadVertexArray;

	std::map<Material*, std::set<RenderableObject, RenderableObject>> Renderer::s_RenderQueue;
	std::set<Ref<Material>> Renderer::s_Materials;
	std::set<Ref<OpenGL::GlVertexArray>> Renderer::s_VertexArrays;

	void Renderer::Init()
	{
		LOG_INFO("Renderer::Init...");
		OpenGL::API::Init();

		// Scene UB
		OpenGL::GlUniformBufferLayout_std140 layout_SceneUB(4 * 4 * 23, {
			{OpenGL::GlShaderDataType::Mat4,	"ViewProjection", 0},
			{OpenGL::GlShaderDataType::Float3,	"CameraPosition", 4 * 4 * 4},
			{OpenGL::GlShaderDataType::Float,	"Time", 4 * 4 * 4 + 4 * 3},
			{OpenGL::GlShaderDataType::Int,		"PointLightCount", 4 * 4 * 5},
			{OpenGL::GlShaderDataType::Struct,	"DirectionalLight", 4 * 4 * 6},
			{OpenGL::GlShaderDataType::Struct,	"PointLight0", 4 * 4 * 8},
			{OpenGL::GlShaderDataType::Struct,	"PointLight1", 4 * 4 * 11},
			{OpenGL::GlShaderDataType::Struct,	"PointLight2", 4 * 4 * 14},
			{OpenGL::GlShaderDataType::Struct,	"PointLight3", 4 * 4 * 17},
			{OpenGL::GlShaderDataType::Struct,	"PointLight4", 4 * 4 * 20},
		});
		s_SceneUB = CreateRef<OpenGL::GlUniformBuffer>(layout_SceneUB);
		s_SceneUB->Bind(0);

		// Model UB
		OpenGL::GlUniformBufferLayout_std140 layout_ModelUB(4 * 4 * 8, {
			{OpenGL::GlShaderDataType::Mat4, "Transform", 0},
			{OpenGL::GlShaderDataType::Mat3, "NormalMatrix", 4 * 4 * 4},
		});
		s_ModelUB = CreateRef<OpenGL::GlUniformBuffer>(layout_ModelUB);
		s_ModelUB->Bind(1);

		// Shader
		s_HDRShader = CreateScope<OpenGL::GlShader>("HDRShader", "assets/shaders/hdr.glsl");
		s_GaussianBlurShader = CreateScope<OpenGL::GlShader>("GaussianBlur", "assets/shaders/GaussianBlur.glsl");

		// HDR FB
		OpenGL::FramebufferSpecification spec_HDR;
		spec_HDR.Height = Application::Get().GetWindow().GetHeight();
		spec_HDR.Width = Application::Get().GetWindow().GetWidth();
		spec_HDR.ColorAttachment.Attach = true;
		spec_HDR.ColorAttachment.Internalformat = 0x881A;
		spec_HDR.ColorAttachment.Format = 0x1908;
		spec_HDR.ColorAttachment.Type = 0x1406;
		spec_HDR.ColorAttachment.Min_Filter = 0x2601;
		spec_HDR.ColorAttachment.Mag_Filter = 0x2601;
		spec_HDR.ColorAttachment.TextureLibrary = true;
		spec_HDR.ColorAttachment.Name = "HDR FrameBuffer (ColorAttachment0)";

		spec_HDR.ColorAttachment1.Attach = true;
		spec_HDR.ColorAttachment1.Internalformat = 0x881A;
		spec_HDR.ColorAttachment1.Format = 0x1908;
		spec_HDR.ColorAttachment1.Type = 0x1406;
		spec_HDR.ColorAttachment1.Min_Filter = 0x2601;
		spec_HDR.ColorAttachment1.Mag_Filter = 0x2601;
		spec_HDR.ColorAttachment1.TextureLibrary = true;
		spec_HDR.ColorAttachment1.Name = "HDR FrameBuffer (ColorAttachment1)";

		spec_HDR.DepthAttachment.Attach = true;
		spec_HDR.DepthAttachment.Internalformat = 0x1902;
		spec_HDR.DepthAttachment.Format = 0x1902;
		spec_HDR.DepthAttachment.Type = 0x1406;
		spec_HDR.DepthAttachment.Min_Filter = 0x2601;
		spec_HDR.DepthAttachment.Mag_Filter = 0x2601;

		s_HDRFramebuffer = CreateRef<OpenGL::GlFramebuffer>(spec_HDR);

		// PingPong FBO
		OpenGL::FramebufferSpecification spec_PingPong;
		spec_PingPong.Height = Application::Get().GetWindow().GetHeight();
		spec_PingPong.Width = Application::Get().GetWindow().GetWidth();
		spec_PingPong.ColorAttachment.Attach = true;
		spec_PingPong.ColorAttachment.Internalformat = 0x881A;
		spec_PingPong.ColorAttachment.Format = 0x1908;
		spec_PingPong.ColorAttachment.Type = 0x1406;
		spec_PingPong.ColorAttachment.Min_Filter = 0x2601;
		spec_PingPong.ColorAttachment.Mag_Filter = 0x2601;
		spec_PingPong.ColorAttachment.Wrap_S = 0x812F;
		spec_PingPong.ColorAttachment.Wrap_T = 0x812F;
		spec_PingPong.ColorAttachment.TextureLibrary = true;
		spec_PingPong.ColorAttachment.Name = "PingPong_1";
		s_PingPongFramebuffer0 = CreateRef<OpenGL::GlFramebuffer>(spec_PingPong);
		spec_PingPong.ColorAttachment.Name = "PingPong_2";
		s_PingPongFramebuffer1 = CreateRef<OpenGL::GlFramebuffer>(spec_PingPong);

		// Quad Vertex Array
		s_QuadVertexArray = CreateScope<OpenGL::GlVertexArray>();

		std::vector<float> vertices = {
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
		};

		std::vector<uint32_t> indices = {
			0, 1, 2,
			3, 2, 1
		};

		Ref<OpenGL::GlVertexBuffer> quadVB = CreateRef<OpenGL::GlVertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
		quadVB->SetLayout({
			{ OpenGL::GlShaderDataType::Float3, "a_Position" },
			{ OpenGL::GlShaderDataType::Float2, "a_TexCoord" },
		});

		Ref<OpenGL::GlIndexBuffer> quadIB = CreateRef<OpenGL::GlIndexBuffer>(indices.data(), 6);

		s_QuadVertexArray->AddVertexBuffer(quadVB);
		s_QuadVertexArray->SetIndexBuffer(quadIB);
	}

	void Renderer::Shutdown()
	{
		LOG_INFO("Renderer2D::Shutdown...");
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		OpenGL::API::SetViewport(0, 0, width, height);
		s_HDRFramebuffer->Resize(width, height);
		s_PingPongFramebuffer0->Resize(width, height);
		s_PingPongFramebuffer1->Resize(width, height);
	}

	void Renderer::BeginScene(const Camera& camera, const DirectionalLight& directionalLight, const std::vector<PointLight>& pointLights)
	{
		s_HDRFramebuffer->Bind();
		OpenGL::API::Clear();
		s_HDRFramebuffer->Unbind();

		// Update SceneUB
		glm::mat4 viewProjectionMatrix = camera.ViewProjection();
		s_SceneUB->SetData(glm::value_ptr(viewProjectionMatrix), "ViewProjection");
		s_SceneUB->SetData(glm::value_ptr(camera.Position()), "CameraPosition");
		const float time = Platform::GetTime();
		s_SceneUB->SetData(&time, "Time");

		// Lights
		{
			const static int offsetDirection = 0;
			const static int offsetColor = 4 * 4 * 1;

			const OpenGL::GlBufferElement& dirLightBE = s_SceneUB->GetElement("DirectionalLight");
			s_SceneUB->SetData(glm::value_ptr(directionalLight.Direction),	 OpenGL::Util::ShaderDataTypeSize(OpenGL::GlShaderDataType::Float3), dirLightBE.Offset + offsetDirection);
			s_SceneUB->SetData(glm::value_ptr(directionalLight.Color),		OpenGL::Util::ShaderDataTypeSize(OpenGL::GlShaderDataType::Float3), dirLightBE.Offset + offsetColor);
		}

		int pointLightCount = std::min(5, (int)pointLights.size());
		s_SceneUB->SetData(&pointLightCount, "PointLightCount");

		for (size_t i = 0; i < pointLightCount; i++)
		{
			const static std::string bufferElementName[] = { "PointLight0", "PointLight1", "PointLight2", "PointLight3", "PointLight4" };
			const static int offsetPosition = 0;
			const static int offsetColor = 4 * 4 * 1;
			const static int offsetConstant = 4 * 4 * 1 + 4 * 3;
			const static int offsetLinear = 4 * 4 * 2;
			const static int offsetQuadratic = 4 * 4 * 2 + 4;

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

	void Renderer::DrawToScreen()
	{
		Blur(20);

		s_HDRShader->Bind();
		s_HDRShader->SetFloat("u_Exposure", s_Exposure);
		s_HDRShader->SetBool("u_HDR", s_HDR);
		s_HDRShader->SetBool("u_Bloom", s_Bloom);
		s_HDRShader->SetBool("u_GammaCorrection", s_GammaCorrection);
		s_HDRFramebuffer->GetColorAttachment()->Bind(0);
		s_PingPongFramebuffer1->GetColorAttachment()->Bind(1);
		s_QuadVertexArray->Bind();
		OpenGL::API::DrawIndexed(s_QuadVertexArray.get(), 6);
		s_QuadVertexArray->Unbind();
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
		s_HDRFramebuffer->Bind();

		for (const auto& material : s_RenderQueue)
		{
			material.first->Bind();

			Ref<OpenGL::GlShader>& shader = ShaderLibrary::Get(material.first->GetShader());

			for (const auto& obj : material.second)
			{
				glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(obj.Transform)));
				s_ModelUB->SetData(glm::value_ptr(obj.Transform), "Transform");
				s_ModelUB->SetData(glm::value_ptr(normalMatrix), "NormalMatrix");

				obj.VertexArray->Bind();
				OpenGL::API::DrawIndexed(obj.VertexArray, obj.VertexArray->GetIndexBuffer()->GetCount());
			}
		}
		OpenGL::API::UnbindVertexArray();

		s_HDRFramebuffer->Unbind();
	}

	void Renderer::Blur(uint32_t amount)
	{
		s_GaussianBlurShader->Bind();

		bool horizontal = true;
		bool first_iteration = true;
		s_QuadVertexArray->Bind();

		for (unsigned int i = 0; i < amount; i++)
		{
			s_GaussianBlurShader->SetBool("u_Horizontal", horizontal);

			if (horizontal)
			{
				s_PingPongFramebuffer0->Bind();
				s_PingPongFramebuffer1->GetColorAttachment()->Bind();

				if (first_iteration)
				{
					s_HDRFramebuffer->GetColorAttachment(1)->Bind();
					first_iteration = false;
				}
			}
			else
			{
				s_PingPongFramebuffer1->Bind();
				s_PingPongFramebuffer0->GetColorAttachment()->Bind();
			}

			// render
			OpenGL::API::DrawIndexed(s_QuadVertexArray.get(), 6);
			horizontal = !horizontal;
		}

		s_QuadVertexArray->Unbind();
		s_PingPongFramebuffer0->Unbind();
	}
}