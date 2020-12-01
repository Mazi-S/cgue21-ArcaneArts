#pragma once

#include "Engine.h"

class MagicBall : public Engine::ScriptableEntity
{
public:
	MagicBall(Engine::Entity entity, glm::vec3 position, glm::vec3 velocity, float lifetime = 3.0f)
		: ScriptableEntity(entity), m_Position(position), m_Velocity(velocity), m_Lifetime(lifetime)
	{ }

	virtual void OnCreate() override
	{
		auto& translation = GetComponent<Engine::TransformComponent>().Translation;
		translation += m_Position;
		GetComponent<Engine::TransformComponent>().Scale = glm::vec3(0.3f, 0.3f, 0.3f);
	}

	virtual void OnUpdate(Engine::Timestep ts) override
	{
		auto& translation = GetComponent<Engine::TransformComponent>().Translation;
		translation += m_Velocity * float(ts);

		m_Lifetime -= ts;

		// just for testing
		if (m_Lifetime < 0.3f)
		{
			static Engine::Ref<Engine::Material> material = Engine::Material::Create(Engine::MaterialProperties("RedMagicBallMaterial", { 0.8f, 0.12f, 0.18f }), Engine::ShaderLibrary::Get("ColorShader"));
			glm::vec3 scale = glm::vec3(2.0f + 10.0f * m_Lifetime);
			GetComponent<Engine::TransformComponent>().Scale = scale;
			GetComponent<Engine::MaterialComponent>().Material = material;
		}

		if (m_Lifetime < 0.0f)
			Destroy();
	}

private:
	float m_Lifetime;
	glm::vec3 m_Position;
	glm::vec3 m_Velocity;
};