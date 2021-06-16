#pragma once

#include "Material.h"
#include "Camera.h"
#include "Light.h"
#include "RenderableObject.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Platform/OpenGL/OpenGLMesh.h"

#include <map>
#include <set>
#include <string>
#include <glm/glm.hpp>

namespace Engine {

	class Renderer
	{
		friend class RendererPanel;

	public:
		static void Init();
		static void Shutdown();

		static void SetExposure(float exposure);
		static void SetHDR(bool hdr) { s_HDR = hdr; }
		static void SetBloom(bool bloom) { s_Bloom = bloom; }
		static void SetGammaCorrection(bool correction) { s_GammaCorrection = correction; }

		static float GetExposure() { return s_Exposure; }
		static bool GetHDR() { return s_HDR; }
		static bool GetBloom() { return s_Bloom; }

		static Ref<OpenGL::GlFramebuffer> GetFramebuffer() { return s_HDRFramebuffer; }

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Camera& camera, const DirectionalLight& directionalLight = DirectionalLight(), const std::vector<PointLight>& pointLights = {});
		static void EndScene();
		static void DrawToScreen();

		static void Submit(const Ref<OpenGL::GlVertexArray>& vertexArray, const Ref<Material>& material, const glm::mat4& transform);
		static void Submit(const std::string& meshName, const std::vector<std::string>& materials, const glm::mat4& transform);

	private:
		static void Render();
		static void Blur(uint32_t amount);

	private:
		static float s_Exposure;
		static bool s_GammaCorrection;
		static bool s_HDR;
		static bool s_Bloom;

		static Ref<OpenGL::GlFramebuffer> s_HDRFramebuffer;
		static Ref<OpenGL::GlFramebuffer> s_PingPongFramebuffer0;
		static Ref<OpenGL::GlFramebuffer> s_PingPongFramebuffer1;

		static Scope<OpenGL::GlShader> s_HDRShader;
		static Scope<OpenGL::GlShader> s_GaussianBlurShader;
		static Scope<OpenGL::GlVertexArray> s_QuadVertexArray;

		// shared over all shaders
		static Ref<OpenGL::GlUniformBuffer> s_SceneUB;
		static Ref<OpenGL::GlUniformBuffer> s_ModelUB;

		static std::map<Material*, std::set<RenderableObject, RenderableObject>> s_RenderQueue;

		static std::set<Ref<Material>> s_Materials;
		static std::set<Ref<OpenGL::GlVertexArray>> s_VertexArrays;
	};

}
