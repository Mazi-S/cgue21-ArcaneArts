#pragma once

#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Camera.h"

// TODO: remove (temporary)
using Texture = std::string;

namespace Engine {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Ref<Camera>& camera, const glm::mat4& transform);
		static void EndScene() {};

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const Ref<Texture>& texture, const glm::mat4& transform = glm::mat4(1.0f));
		
		static void Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

	private:
		// shared over all shaders
		static Ref<UniformBuffer> s_SceneUB;
	};

}
