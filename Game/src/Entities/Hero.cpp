#include "Hero.h"

void Hero::OnEvent(Engine::Event& e)
{
	Engine::EventHandler eventHandler(e);
	eventHandler.Handle<Engine::MouseButtonPressedEvent>(EG_BIND_EVENT_FN(Hero::OnMouseButtonPressed));
	eventHandler.Handle<Engine::MouseScrolledEvent>(EG_BIND_EVENT_FN(Hero::OnMouseScrolled));
}

bool Hero::OnMouseButtonPressed(Engine::MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == Engine::Mouse::ButtonLeft)
		UseLeftHand();

	if (e.GetMouseButton() == Engine::Mouse::ButtonRight)
		UseRightHand();

	return false;
}

bool Hero::OnMouseScrolled(Engine::MouseScrolledEvent& e)
{
	if (m_AktiveSpell == MagicBallType::Fire)
		m_AktiveSpell = MagicBallType::Water;
	else
		m_AktiveSpell = MagicBallType::Fire;

	if (m_LeftHand)
		DropLeft();

	CreateMagicBall(m_AktiveSpell);

	return false;
}

void Hero::UseLeftHand()
{
	if (m_LeftHand)
		ThrowLeft();
	else
		CreateMagicBall(m_AktiveSpell);
}

void Hero::UseRightHand()
{
	if (m_RightHand)
		DropRight();
	else
		CreateMagicBall(MagicBallType::Light);
}

void Hero::CreateMagicBall(MagicBallType type)
{
	Engine::Entity ball = m_Scene->CreateEntity("MagicBall");
	ball.AddComponent<Engine::ParentComponent>(m_EntityHandle);
	auto& tc = ball.GetComponent<Engine::TransformComponent>();
	tc.Scale = { 0.1f, 0.1f, 0.1f };

	switch (type)
	{
	case MagicBallType::Fire:
		// left hand
		ball.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Fire"));
		ball.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Sphere"));
		ball.AddNativeScript<MagicBall>();
		ball.GetComponent<Engine::NativeScriptComponent>().Active = false;
		tc.Translation = { -0.5f, -0.2f, -1.0f };
		m_LeftHand = ball;
		break;
	case MagicBallType::Water:
		// left hand
		ball.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Water"));
		ball.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Sphere"));
		ball.AddNativeScript<MagicBall>();
		ball.GetComponent<Engine::NativeScriptComponent>().Active = false;
		tc.Translation = { -0.5f, -0.2f, -1.0f };
		m_LeftHand = ball;
		break;
	case MagicBallType::Light:
		// right hand
		ball.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Light"));
		ball.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Sphere"));
		tc.Translation = { 0.5f, -0.2f, -1.0f };
		m_RightHand = ball;
		break;
	}
}

void Hero::ThrowLeft()
{
	// todo: System::RemoveParent()
		auto& tc = GetComponent<Engine::TransformComponent>();
		auto& tc_lh = m_LeftHand.GetComponent<Engine::TransformComponent>();
		tc_lh.Translation = Engine::System::Util::Transform(tc, tc_lh.Translation);
		tc_lh.Rotation += tc.Rotation;
		tc_lh.Scale *= tc.Scale;
		m_LeftHand.RemoveComponent<Engine::ParentComponent>();

	m_LeftHand.GetComponent<Engine::NativeScriptComponent>().Active = true;
	glm::vec4 velocity = glm::toMat4(glm::quat(tc.Rotation)) * glm::vec4{ 1.0f, 1.0f, -50.0f, 0.0 };
	m_LeftHand.AddComponent<Engine::VelocityComponent>(glm::vec3{ velocity.x, velocity.y, velocity.z });

	m_LeftHand = Engine::Entity();
}

void Hero::DropLeft()
{
	m_LeftHand.Destroy();
	m_LeftHand = Engine::Entity();
}

void Hero::DropRight()
{
	m_RightHand.Destroy();
	m_RightHand = Engine::Entity();
}

