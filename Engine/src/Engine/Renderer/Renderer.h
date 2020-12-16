#pragma once

#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Light.h"

namespace Engine {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Camera& camera, const DirectionalLight& directionalLight = DirectionalLight(), const PointLight& pointLight = PointLight());
		static void EndScene() {};

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Material>& material, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const glm::mat4& transform = glm::mat4(1.0f));

	private:
		// shared over all shaders
		static Ref<UniformBuffer> s_SceneUB;
	};

}
