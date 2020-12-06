#pragma once

#include "Engine.h"

enum class MagicBallType
{
	Fire,
	Water,
	Light
};

class Hero : public Engine::ScriptableEntity
{
public:
	Hero() = default;

	virtual void OnEvent(Engine::Event e) override
	{
		Engine::EventHandler eventHandler(e);
		eventHandler.Handle<Engine::MouseButtonPressedEvent>(EG_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));
	}

	bool OnMouseButtonPressed(Engine::MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Engine::Mouse::ButtonLeft)
			UseLeftHand();

		if (e.GetMouseButton() == Engine::Mouse::ButtonRight)
			UseLeftHand();

		return false;
	}

private:

	void UseLeftHand()
	{
		// if empty
		CreateMagicBall(MagicBallType::Light);
	}

	void UseRightHand()
	{
		// if empty
		CreateMagicBall(MagicBallType::Fire);
	}

	void CreateMagicBall(MagicBallType type);
	void ThrowLeft();
	void ThrowRight();

private:
	// todo
	Engine::Entity LeftHand; // add null entity
	Engine::Entity RightHand; // add null entity
};

class MagicBall : public Engine::ScriptableEntity
{
public:
	MagicBall(Engine::Entity entity, float lifetime = 3.0f)
		: ScriptableEntity(entity), m_Lifetime(lifetime)
	{ }

	virtual void OnUpdate(Engine::Timestep ts) override
	{
		auto& translation = GetComponent<Engine::TransformComponent>().Translation;
		translation += m_Velocity * float(ts);

		m_Lifetime -= ts;

		if (m_Lifetime < 0.3f)
		{
			glm::vec3 scale = glm::vec3(2.0f + 10.0f * m_Lifetime);
			GetComponent<Engine::TransformComponent>().Scale = scale;
			GetComponent<Engine::MaterialComponent>().Material = Engine::MaterialLibrary::Get("MagicBallRed");
			m_Velocity = { 0.0f, 0.0f, 0.0f };
		}

		if (m_Lifetime < 0.0f)
			Destroy();
	}

private:
	bool m_Active = false;
	float m_Lifetime;
	glm::vec3 m_Velocity{ 0.0f, 0.0f, 50.0f };
};
