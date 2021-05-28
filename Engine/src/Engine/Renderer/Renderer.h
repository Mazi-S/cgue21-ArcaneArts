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
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Camera& camera, const DirectionalLight& directionalLight = DirectionalLight(), const PointLight& pointLight = PointLight());
		static void EndScene();

		static void Submit(const Ref<OpenGL::GlVertexArray>& vertexArray, const Ref<Material>& material, const glm::mat4& transform);
		static void Submit(const std::string& meshName, const std::vector<std::string>& materials, const glm::mat4& transform);

	private:
		static void Render();

	private:
		// shared over all shaders
		static Ref<OpenGL::GlUniformBuffer> s_SceneUB;

		//static std::

		static std::map<Material*, std::set<RenderableObject, RenderableObject>> s_RenderQueue;

		static std::set<Ref<Material>> s_Materials;
		static std::set<Ref<OpenGL::GlVertexArray>> s_VertexArrays;
	};

}
