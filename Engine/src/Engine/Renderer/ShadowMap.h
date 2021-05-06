#pragma once
#include "Engine/Core/Base.h"

#include "Light.h"
#include "Camera.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "Platform/OpenGL/OpenGLMesh.h"

#include <map>
#include <string>
#include <set>
#include <glm/glm.hpp>

namespace Engine {

	struct ShadableObject
	{
		const OpenGL::GlVertexArray* VertexArray = nullptr;
		const glm::mat4 Transform = glm::mat4(1);

		ShadableObject() = default;

		ShadableObject(OpenGL::GlVertexArray* vertexArray, const glm::mat4 & transform)
			: VertexArray(vertexArray), Transform(transform) { }

		bool operator()(const ShadableObject& left, const ShadableObject& right) const;
	};

	class ShadowMap
	{
	public:
		static void Init();

		static void BeginScene(const Camera& camera, const DirectionalLight& directionalLight = DirectionalLight());
		static void EndScene();

		static void Submit(const Ref<OpenGL::GlVertexArray>& vertexArray, const glm::mat4& transform);
		static void Submit(const std::string& meshName, const glm::mat4& transform);
	
	private:
		static void UpdateShadowMap();

	private:
		static Ref<OpenGL::GlFramebuffer> s_Framebuffer;
		static Ref<OpenGL::GlShader> s_Shader;

		static std::map<OpenGL::GlVertexArray*, std::set<ShadableObject, ShadableObject>> s_DepthQueue;
		static std::set<Ref<OpenGL::GlVertexArray>> s_VertexArrays;
	
		static Ref<OpenGL::GlUniformBuffer> s_UniformBuffer;
	};

}