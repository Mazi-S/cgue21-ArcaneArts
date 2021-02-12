#include "Hero.h"
#include "Engine/Physics/Physics.h"

void Hero::OnEvent(Engine::Event& e)
{
	Engine::EventHandler eventHandler(e);
	eventHandler.Handle<Engine::MouseButtonPressedEvent>(EG_BIND_EVENT_FN(Hero::OnMouseButtonPressed));
	eventHandler.Handle<Engine::MouseScrolledEvent>(EG_BIND_EVENT_FN(Hero::OnMouseScrolled));
}

bool Hero::OnMouseButtonPressed(Engine::MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == Engine::Mouse::ButtonLeft)
		UseRightHand();

	if (e.GetMouseButton() == Engine::Mouse::ButtonRight)
		UseLeftHand();

	return false;
}

bool Hero::OnMouseScrolled(Engine::MouseScrolledEvent& e)
{
	if (m_AktiveSpell == MagicBallType::Fire)
		m_AktiveSpell = MagicBallType::Water;
	else
		m_AktiveSpell = MagicBallType::Fire;

	if (m_RightHand)
		DropRight();

	CreateMagicBall(m_AktiveSpell);

	return false;
}

void Hero::UseLeftHand()
{
	if (m_LeftHand)
		DropLeft();
	else
		CreateMagicBall(MagicBallType::Light);
}

void Hero::UseRightHand()
{
	if (m_RightHand)
		ThrowRight();
	else
		CreateMagicBall(m_AktiveSpell);
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
		ball.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Fire"));
		ball.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Sphere"));
		ball.AddNativeScript<MagicBall>();
		ball.GetComponent<Engine::NativeScriptComponent>().Active = false;
		tc.Translation = { 0.5f, -0.2f, -1.0f }; // right hand
		m_RightHand = ball;
		break;
	case MagicBallType::Water:
		ball.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Water"));
		ball.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Sphere"));
		ball.AddNativeScript<MagicBall>();
		ball.GetComponent<Engine::NativeScriptComponent>().Active = false;
		tc.Translation = { 0.5f, -0.2f, -1.0f }; // right hand
		m_RightHand = ball;
		break;
	case MagicBallType::Light:
		ball.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall_Light"));
		ball.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Sphere"));
		ball.AddComponent<Engine::PointLightComponent>(glm::vec3{0.8f, 0.97f, 0.99f}, 1.0f, 0.4f, 0.1f);
		tc.Translation = { -0.5f, -0.2f, -1.0f }; // left hand
		m_LeftHand = ball;
		break;
	}
}

void Hero::ThrowRight()
{
	auto& tc = GetComponent<Engine::TransformComponent>();
	auto& tc_rh = m_RightHand.GetComponent<Engine::TransformComponent>();
	tc_rh.Translation = Engine::System::Util::Transform(tc, tc_rh.Translation);
	tc_rh.Rotation += tc.Rotation;
	tc_rh.Scale *= tc.Scale;
	m_RightHand.RemoveComponent<Engine::ParentComponent>();

	m_RightHand.GetComponent<Engine::NativeScriptComponent>().Active = true;
	glm::vec4 velocity = glm::toMat4(glm::quat(tc.Rotation)) * glm::vec4{ 1.0f, 1.0f, -30.0f, 0.0 };
	auto actor = Engine::PhysicsAPI::CreateRegidDynamicSphere(tc_rh.Translation, tc_rh.Scale.x);
	actor->setLinearVelocity({ velocity.x, velocity.y, velocity.z });
	actor->setMass(0.5f);
	m_RightHand.AddComponent<Engine::RegidDynamicComponent>(actor);

	m_RightHand = Engine::Entity();
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

