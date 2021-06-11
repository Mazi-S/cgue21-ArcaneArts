#include "egpch.h"
#include "ParticleSystem.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui.h>
#include "Engine/ImGui/ImGuiUtil.h"

#include "glad/glad.h"
namespace Engine {

	Scope<OpenGL::GlShader> ParticleSystem::s_FireUpdateShader;
	Scope<OpenGL::GlShader> ParticleSystem::s_MagicBallUpdateShader;
	Scope<OpenGL::GlShader> ParticleSystem::s_BillboardShader;
	Scope<OpenGL::GlTexture2D> ParticleSystem::s_ParticleTexture;
	Scope<OpenGL::GlTexture1D> ParticleSystem::s_RandomTexture;

	void ParticleSystem::Init()
	{
		// Update Shader
		s_FireUpdateShader = CreateScope<OpenGL::GlShader>("FireUpdateShader", "assets/shaders/ParticleSystem_update_fire.glsl", false);
		s_MagicBallUpdateShader = CreateScope<OpenGL::GlShader>("MagicBallUpdateShader", "assets/shaders/ParticleSystem_update_magicBall.glsl", false);

		char* varyings[4];
		varyings[0] = "Type";
		varyings[1] = "Position";
		varyings[2] = "Velocity";
		varyings[3] = "Power";

		s_FireUpdateShader->SetTransformFeedbackVaryings(4, varyings, 0x8C8C);
		s_FireUpdateShader->Link();

		s_MagicBallUpdateShader->SetTransformFeedbackVaryings(4, varyings, 0x8C8C);
		s_MagicBallUpdateShader->Link();

		// Billboard Shader
		s_BillboardShader = CreateScope<OpenGL::GlShader>("BillboardShader", "assets/shaders/ParticleSystem_billboard.glsl");

		OpenGL::Texture1DSpecification spec;
		spec.Width = 1000;
		spec.Type = 0x1406;
		spec.Internalformat = 0x1907;
		spec.Format = 0x1907;
		spec.Min_Filter = 0x2601;
		spec.Mag_Filter = 0x2601;
		spec.Wrap_S = 0x2901;

		glm::vec3* randomData = new glm::vec3[spec.Width];
		for (uint32_t i = 0; i < spec.Width; i++) {
			randomData[i].x = Random();
			randomData[i].y = Random();
			randomData[i].z = Random();
		}

		s_RandomTexture = CreateScope<OpenGL::GlTexture1D>("RandomTexture", spec);
		
		s_RandomTexture->SetData(randomData, sizeof(randomData));
		delete[] randomData;

		// Particle Texture
		s_ParticleTexture = CreateScope<OpenGL::GlTexture2D>("ParticleTexture", "assets/textures/particle_mask.png");
	}


	struct Particle
	{
		float Type;
		glm::vec3 Position;
		glm::vec3 Velocity;
		float Power;
	};

	Engine::ParticleSystem::ParticleSystem(ParticleSystemType type, const glm::vec3& position, float emitPower, float cooling, float particleSize, glm::vec4 colorStart, glm::vec4 colorEnd)
		: m_Type(type), m_Position(position), m_EmitPower(emitPower), m_Cooling(cooling), m_ParticleSize(particleSize), m_ColorStart(colorStart), m_ColorEnd(colorEnd)
	{
		m_Time = 0;
		m_isFirst = true;

		m_currVB = 0;
		m_currTFB = 1;

		InitParticleSystem();
	}

	Engine::ParticleSystem::~ParticleSystem()
	{
		delete m_VertexBuffer[0];
		delete m_VertexBuffer[1];
		delete m_TransformFeedback[0];
		delete m_TransformFeedback[1];
	}

	void ParticleSystem::OnUpdate(Timestep timestep)
	{
		Swap();

		m_Time += timestep.GetMilliseconds();

		OpenGL::API::UnbindVertexArray();

		OpenGL::GlShader* shader = s_FireUpdateShader.get();
		if (m_Type == ParticleSystemType::MagicBall)
			shader = s_MagicBallUpdateShader.get();

		shader->Bind();
		shader->SetFloat("u_DeltaTime_ms", timestep.GetMilliseconds());
		shader->SetFloat("u_Time", m_Time);
		shader->SetFloat3("u_EmitterPosition", m_Position);
		shader->SetFloat("u_EmitPower", m_EmitPower);
		shader->SetFloat("u_Cooling", m_Cooling);

		s_RandomTexture->Bind();

		m_VertexBuffer[m_currVB]->Bind();
		m_TransformFeedback[m_currTFB]->Bind();

		glEnable(GL_RASTERIZER_DISCARD);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);					// type
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);	// position
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16);	// velocity
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28);	// power

		glBeginTransformFeedback(GL_POINTS);

		if (m_isFirst) {
			glDrawArrays(GL_POINTS, 0, 1);

			m_isFirst = false;
		}
		else {
			glDrawTransformFeedback(GL_POINTS, m_TransformFeedback[m_currVB]->GetRendererID());
		}

		glEndTransformFeedback();

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);

		glDisable(GL_RASTERIZER_DISCARD);
	}

	void ParticleSystem::OnUpdate(Timestep timestep, const glm::vec3& position, float emitPower, float cooling, float particleSize, glm::vec4 colorStart, glm::vec4 colorEnd)
	{
		m_Position = position;
		m_EmitPower = emitPower;
		m_Cooling = cooling;
		m_ParticleSize = particleSize;
		m_ColorStart = colorStart;
		m_ColorEnd = colorEnd;

		OnUpdate(timestep);
	}

	void ParticleSystem::OnRender(const glm::mat4& viewProjection, const glm::vec3& cameraPos)
	{
		s_BillboardShader->Bind();
		s_BillboardShader->SetFloat3("u_CameraPos", cameraPos);
		s_BillboardShader->SetMat4("u_ViewProjection", viewProjection);
		s_BillboardShader->SetFloat("u_BillboardSize", m_ParticleSize);
		s_BillboardShader->SetFloat4("u_ColorStart", m_ColorStart);
		s_BillboardShader->SetFloat4("u_ColorEnd", m_ColorEnd);
		if (m_Type == ParticleSystemType::MagicBall)
		{
			// TODO: improve
			static float r = m_Time / 10000;
			glm::mat4 transform = glm::translate(glm::mat4(1), m_Position) * glm::rotate(glm::mat4(1), r, {0,1,0});
			s_BillboardShader->SetMat4("u_Offset", transform);
		}
		else
			s_BillboardShader->SetMat4("u_Offset", glm::mat4(1));

		s_ParticleTexture->Bind();


		m_VertexBuffer[m_currTFB]->Bind();

		glDepthMask(GL_FALSE);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);	// position
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28);	// power

		glDrawTransformFeedback(GL_POINTS, m_TransformFeedback[m_currTFB]->GetRendererID());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDepthMask(GL_TRUE);
	}

#define MAX_PARTICLES 10000000

	void ParticleSystem::InitParticleSystem()
	{
		uint32_t size = sizeof(Particle) * MAX_PARTICLES;

		Particle particles[10];
		memset(particles, 0, sizeof(particles));
		particles[0].Type = 0.0f;
		particles[0].Position = m_Position;
		particles[0].Velocity = glm::vec3(0.0f, 0.0001f, 0.0f);
		particles[0].Power = 0.0f;

		m_VertexBuffer[0] = new OpenGL::GlVertexBuffer(size);
		m_VertexBuffer[1] = new OpenGL::GlVertexBuffer(size);
		m_VertexBuffer[0]->SetData(&particles, sizeof(particles), 0);
		m_VertexBuffer[1]->SetData(&particles, sizeof(particles), 0);

		m_TransformFeedback[0] = new OpenGL::GlTransformFeedback();
		m_TransformFeedback[1] = new OpenGL::GlTransformFeedback();

		m_TransformFeedback[0]->BindBuffer(*m_VertexBuffer[0], 0);
		m_TransformFeedback[1]->BindBuffer(*m_VertexBuffer[1], 0);
	}

	void ParticleSystem::Swap()
	{
		m_currVB = m_currTFB;
		m_currTFB = (m_currTFB + 1) & 0x1;
	}

	float ParticleSystem::Random()
	{
		float Max = RAND_MAX;
		return ((float)rand() / Max);
	}

}
