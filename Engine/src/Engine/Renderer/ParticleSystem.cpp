#include "egpch.h"
#include "ParticleSystem.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "glad/glad.h"
namespace Engine {

	Scope<OpenGL::GlShader> ParticleSystem::s_UpdateShader;
	Scope<OpenGL::GlShader> ParticleSystem::s_BillboardShader;
	Scope<OpenGL::GlTexture2D> ParticleSystem::s_ParticleTexture;
	Scope<OpenGL::GlTexture1D> ParticleSystem::s_RandomTexture;

	void ParticleSystem::Init()
	{
		// Update Shader
		s_UpdateShader = CreateScope<OpenGL::GlShader>("UpdateShader", "assets/shaders/ParticleSystem_update.glsl", false);

		char* varyings[4];
		varyings[0] = "Type1";
		varyings[1] = "Position1";
		varyings[2] = "Velocity1";
		varyings[3] = "Age1";

		s_UpdateShader->SetTransformFeedbackVaryings(4, varyings, 0x8C8C);
		s_UpdateShader->Link();

		s_UpdateShader->Bind();

		s_UpdateShader->SetInt("u_RandomTexture", 3);

		s_UpdateShader->SetFloat("u_LauncherLifetime", 100.0f);
		s_UpdateShader->SetFloat("u_ShellLifetime", 10000.0f);
		s_UpdateShader->SetFloat("u_SecondaryShellLifetime", 2500.0f);

		// Billboard Shader
		s_BillboardShader = CreateScope<OpenGL::GlShader>("BillboardShader", "assets/shaders/ParticleSystem_billboard.glsl");
		s_BillboardShader->Bind();
		s_BillboardShader->SetInt("u_ColorMap", 0);
		s_BillboardShader->SetFloat("u_BillboardSize", 0.01f);

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
		s_ParticleTexture = CreateScope<OpenGL::GlTexture2D>("ParticleTexture", "assets/textures/fireworks_red.png");
	}

#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 10.0f

	struct Particle
	{
		float Type;
		glm::vec3 Pos;
		glm::vec3 Vel;
		float LifetimeMillis;
	};

	Engine::ParticleSystem::ParticleSystem()
	{
		m_Time = 0;
		m_isFirst = true;

		m_currVB = 0;
		m_currTFB = 1;
	}

	Engine::ParticleSystem::~ParticleSystem()
	{
		delete m_VertexBuffer[0];
		delete m_VertexBuffer[1];
		delete m_TransformFeedback[0];
		delete m_TransformFeedback[1];
	}

	void ParticleSystem::InitParticleSystem(const glm::vec3& Pos)
	{
		uint32_t size = sizeof(Particle) * MAX_PARTICLES;

		Particle particles[MAX_PARTICLES];
		memset(particles, 0, sizeof(particles));
		particles[0].Type = 0.0f;
		particles[0].Pos = Pos;
		particles[0].Vel = glm::vec3(0.0f, 0.0001f, 0.0f);
		particles[0].LifetimeMillis = 0.0f;

		m_VertexBuffer[0] = new OpenGL::GlVertexBuffer(size);
		m_VertexBuffer[1] = new OpenGL::GlVertexBuffer(size);
		m_VertexBuffer[0]->SetData(&particles, sizeof(particles), 0);
		m_VertexBuffer[1]->SetData(&particles, sizeof(particles), 0);

		m_TransformFeedback[0] = new OpenGL::GlTransformFeedback();
		m_TransformFeedback[1] = new OpenGL::GlTransformFeedback();


		m_TransformFeedback[0]->BindBuffer(*m_VertexBuffer[0], 0);
		m_TransformFeedback[1]->BindBuffer(*m_VertexBuffer[1], 0);
	}

	void ParticleSystem::Render(Timestep timestep, const glm::mat4& viewProjection, const glm::vec3& cameraPos)
	{
		m_Time += timestep.GetMilliseconds();
		UpdateParticles(timestep);

		RenderParticles(viewProjection, cameraPos);

		Swap();
	}


	void ParticleSystem::UpdateParticles(Timestep timestep)
	{
		s_UpdateShader->Bind();
		s_UpdateShader->SetFloat("u_DeltaTimeMillis", timestep.GetMilliseconds());
		s_UpdateShader->SetFloat("u_Time", m_Time);

		s_RandomTexture->Bind(3);
		glEnable(GL_RASTERIZER_DISCARD);

		m_VertexBuffer[m_currVB]->Bind();
		m_TransformFeedback[m_currTFB]->Bind();

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);					// type
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);	// position
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16);	// velocity
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28);	// lifetime

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
	}

	void ParticleSystem::RenderParticles(const glm::mat4& viewProjection, const glm::vec3& cameraPos)
	{
		s_BillboardShader->Bind();
		s_BillboardShader->SetFloat3("u_CameraPos", cameraPos);
		s_BillboardShader->SetMat4("u_ViewProjection", viewProjection);
		s_ParticleTexture->Bind();

		glDisable(GL_RASTERIZER_DISCARD);

		m_VertexBuffer[m_currTFB]->Bind();

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);  // position

		glDrawTransformFeedback(GL_POINTS, m_TransformFeedback[m_currTFB]->GetRendererID());

		glDisableVertexAttribArray(0);
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
