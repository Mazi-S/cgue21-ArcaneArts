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
		ParticleSystem(const glm::vec3& position, float emitPower = 0.001f, float cooling = 0.05f, float particleSize = 0.01f, glm::vec4 colorStart = { 1, 1, 0, 1 }, glm::vec4 colorEnd = { 1, 0, 0, 0 });
		~ParticleSystem();
		
		void OnUpdate(Timestep timestep);
		void OnUpdate(Timestep timestep, const glm::vec3& position, float emitPower, float cooling, float particleSize, glm::vec4 colorStart, glm::vec4 colorEnd);
		void OnRender(const glm::mat4& viewProjection, const glm::vec3& cameraPos);

		static void Init();

	private:
		void InitParticleSystem();

		void Swap();
	public:
		static float Random();

	private:
		float m_Time;
		bool m_isFirst;

		glm::vec3 m_Position;
		float m_EmitPower;
		float m_Cooling;

		float m_ParticleSize;
		glm::vec4 m_ColorStart;
		glm::vec4 m_ColorEnd;

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