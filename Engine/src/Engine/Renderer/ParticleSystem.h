#pragma once

#include "Engine/Core/Timestep.h"
#include <glm/glm.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLTransformFeedback.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLTexture1D.h"

namespace Engine {

	class ParticleSystem
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		void InitParticleSystem(const glm::vec3& Pos);

		void Render(Timestep timestep, const glm::mat4& viewProjection, const glm::vec3& cameraPos);

		static void Init();

	private:
		void UpdateParticles(Timestep timestep);
		void RenderParticles(const glm::mat4& viewProjection, const glm::vec3& cameraPos);

		void Swap();
	public:
		static float Random();
	private:
		float m_Time;
		bool m_isFirst;

		uint16_t m_currVB;
		uint16_t m_currTFB;

		OpenGL::GlVertexBuffer* m_VertexBuffer[2];
		OpenGL::GlTransformFeedback* m_TransformFeedback[2];

		static Scope<OpenGL::GlShader> s_UpdateShader;
		static Scope<OpenGL::GlShader> s_BillboardShader;
		static Scope<OpenGL::GlTexture2D> s_ParticleTexture;
		static Scope<OpenGL::GlTexture1D> s_RandomTexture;
	};
}