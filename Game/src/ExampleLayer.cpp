#include "ExampleLayer.h"

#include <Engine/Events/KeyEvent.h>
#include <Engine/Util/ObjectLoader.h>

#include "Entities/HeroScript.h"
#include "Entities/MagicBallScript.h"
#include "Entities/MonsterScript.h"

#include "Components/GameComponents.h"

#include "ActorFactory.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui/imgui.h"

ExampleLayer::ExampleLayer()
	: Layer("Example")
{ }

void ExampleLayer::OnAttach()
{
	// Init here
	Engine::OpenGL::API::SetClearColor({ 0.0f, 0.3f, 0.5f, 1.0f });

	// Create Scene
	m_Scene = Engine::CreateRef<Engine::Scene>();

	Engine::SceneSerializer sceneSerializer = Engine::SceneSerializer(m_Scene);
	sceneSerializer.Deserialize("assets/scenes/Example.yaml");

	// Hero
	{
		auto view = m_Scene->m_Registry.view<Engine::Component::Physics::CharacterControllerComponent>();
		m_Hero = Engine::Entity{ *view.begin(), &m_Scene->m_Registry };
	
		m_Hero.AddComponent<HeroComponent>();
		m_Hero.AddNativeScript<HeroScript>();
		Engine::System::Util::Activate(m_Hero.GetComponent<Engine::Component::Physics::CharacterControllerComponent>());
	}

	// Skybox
	m_Skybox = Engine::CreateRef<Engine::Skybox>();

	// Seed
	srand(420);

	// Monster
	if (true) {
		// Big Monsters
		for (size_t i = 0; i < 2; i++)
		{
			Engine::Entity entity = m_Scene->CreateEntity("Monster [Big]");
			auto mesh = "Monster";
			glm::vec3 t{ (rand() % 100) - 50, 5.5f, (rand() % 100) - 50 };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.0f, 1.0f, 1.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("MonsterBigMaterial");
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			auto actor = ActorFactory::Monster(mesh, t, r, s);
			entity.AddComponent<Engine::Component::Physics::RigidDynamicComponent>(actor);
			entity.AddComponent<MonsterComponent>(Engine::SoundLibrary::Get("Monster"), Engine::SoundLibrary::Get("MonsterDying"));
			entity.AddNativeScript<MonsterScript>();
		}
		// Small Monsters
		for (size_t i = 0; i < 4; i++)
		{
			Engine::Entity entity = m_Scene->CreateEntity("Monster [Small]");
			std::string mesh = "Monster";
			glm::vec3 t{ (rand() % 100) - 50, 5.5f, (rand() % 100) - 50 };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 0.5f, 0.5f, 0.5f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("MonsterSmallMaterial");
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			auto actor = ActorFactory::Monster(mesh, t, r, s);
			entity.AddComponent<Engine::Component::Physics::RigidDynamicComponent>(actor);
			entity.AddComponent<MonsterComponent>(Engine::SoundLibrary::Get("Monster"), Engine::SoundLibrary::Get("MonsterDying"), 35.0f, 10.0f, 4.5f, 30.0f, 2.5f);
			entity.AddNativeScript<MonsterScript>();
		}
	}

	// ParticleSystem
	Engine::ParticleSystem::Init();
	m_ParticleSystem = Engine::CreateRef<Engine::ParticleSystem>();
	m_ParticleSystem->InitParticleSystem({ 0, 1, 3 });

	// ImGui
	m_MaterialPanel = Engine::CreateScope<Engine::MaterialPanel>();
	m_ShaderPanel = Engine::CreateScope<Engine::ShaderPanel>();
	m_TexturePanel = Engine::CreateScope<Engine::Texture2DPanel>();
	m_MeshPanel = Engine::CreateScope<Engine::MeshPanel>();
	m_SceneHierarchyPanel = Engine::CreateScope<Engine::SceneHierarchyPanel>(m_Scene);
}

void ExampleLayer::OnDetach()
{
	// Shutdown here
	m_Scene = nullptr;
}

void ExampleLayer::OnUpdate(Engine::Timestep ts)
{
	// Update here
	m_Scene->OnUpdate(ts);

	// Render here

	// Render Skybox and CubeMap
	m_Skybox->Draw(m_Scene->GetCamera());

	// Render Scene
	m_Scene->OnRender();

	auto camera = m_Scene->GetCamera();
	glm::vec3 cameraPos = { camera.Transform[3][0], camera.Transform[3][1], camera.Transform[3][2] };
	glm::mat4 viewProjectionMatrix = camera.Projection * glm::inverse(camera.Transform);

	m_ParticleSystem->Render(ts, viewProjectionMatrix, cameraPos);
}

void ExampleLayer::OnEvent(Engine::Event& event)
{
	m_Scene->OnEvent(event);

	Engine::EventHandler eventHandler(event);
	eventHandler.Handle<Engine::KeyPressedEvent>(EG_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
	eventHandler.HandleGameEvent<GameEndEvent>(EG_BIND_EVENT_FN(ExampleLayer::OnGameEnd));
}

bool ExampleLayer::OnKeyPressed(Engine::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == Engine::Key::Escape)
	{
		if (m_Menu)
		{
			Engine::System::Util::Activate(m_Hero.GetComponent<Engine::Component::Physics::CharacterControllerComponent>());
			m_Menu = false;
		}
		else
		{
			Engine::System::Util::Deactivate(m_Hero.GetComponent<Engine::Component::Physics::CharacterControllerComponent>());
			m_Menu = true;
		}
	}
	return false;
}

bool ExampleLayer::OnGameEnd(GameEndEvent& event)
{
	Engine::Application::Get().Remove(this);
	return false;
}

void ExampleLayer::OnImGui()
{
	ImGui::ShowDemoWindow();

	m_MaterialPanel->OnImGui();
	m_ShaderPanel->OnImGui();
	m_TexturePanel->OnImGui();
	m_MeshPanel->OnImGui();
	m_SceneHierarchyPanel->OnImGui();
}

