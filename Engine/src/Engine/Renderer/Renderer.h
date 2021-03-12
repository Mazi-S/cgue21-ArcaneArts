#pragma once

#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include "RenderableObject.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <map>
#include <set>
#include <glm/glm.hpp>

namespace Engine {

	struct SceneData
	{
		Camera Camera;
		glm::vec3 CameraPos;

		DirectionalLight DirectionalLight;
		PointLight PointLight;
		glm::mat4 DepthViewMatrix;
		glm::mat4 DepthProjectionMatrix;
	};

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Camera& camera, const DirectionalLight& directionalLight = DirectionalLight(), const PointLight& pointLight = PointLight());
		static void EndScene();

		static void Submit(const Ref<OpenGL::GlVertexArray>& vertexArray, const Ref<Material>& material, const glm::mat4& transform, bool shadow);
		static void Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const glm::mat4& transform, bool shadow);

	private:
		static void Render();
		static void UpdateShadowMap();

	private:
		// shared over all shaders
		static Ref<OpenGL::GlUniformBuffer> s_SceneUB;

		static SceneData s_SceneData;

		// Shadow Map
		static Ref<OpenGL::GlFramebuffer> s_ShadowMapFB;
		static Ref<OpenGL::GlShader> s_ShadowMapShader;

		static std::map<Material*, std::set<RenderableObject, RenderableObject>> s_RenderQueue;
		static std::map<OpenGL::GlVertexArray*, std::set<RenderableObject, RenderableObject>> s_DepthQueue;

		static std::set<Ref<Material>> s_Materials;
		static std::set<Ref<OpenGL::GlVertexArray>> s_VertexArrays;
	};

}
