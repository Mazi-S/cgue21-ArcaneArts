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

	// Load Sounds
	{
		Engine::SoundLibrary::Load("FireballShoot", "assets/sounds/fireball-shoot.wav", 0.1f);
		Engine::SoundLibrary::Load("FireballCast", "assets/sounds/fireball-cast.wav", 0.3f);
		Engine::SoundLibrary::Load("LightningShoot", "assets/sounds/lightning-shoot.wav", 0.05f);
		Engine::SoundLibrary::Load("Forest", "assets/sounds/forest.wav", 0.1f);
		Engine::SoundLibrary::Load("LightCast", "assets/sounds/light.wav", 0.4f);
		Engine::SoundLibrary::Load("Walk", "assets/sounds/walk.wav");
		Engine::SoundLibrary::Load("Thunder", "assets/sounds/thunder.wav", 1.0f);
		Engine::SoundLibrary::Load("Impact", "assets/sounds/impact.wav", 0.4f);
		Engine::SoundLibrary::Load("Monster", "assets/sounds/monster.wav", 0.4f);
		Engine::SoundLibrary::Load("MonsterDying", "assets/sounds/monster-dying.wav", 1.0f);
	}

	// Create Scene
	m_Scene = Engine::CreateRef<Engine::Scene>();

	// light
	auto directionalLight = m_Scene->CreateEntity("Moon [Directional Light]");
	directionalLight.AddComponent<Engine::Component::Renderer::DirectionalLightComponent>(glm::vec3{ 0.0f, -1.0f, 0.8f }, glm::vec3{ 0.3f, 0.3f, 0.3f } );

	// Hero
	m_Hero = m_Scene->CreateEntity("Hero");
	m_Hero.AddComponent<Engine::Component::Audio::ListenerComponent>();
	m_Hero.AddComponent<HeroComponent>();
	m_Hero.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { 0.0f, 8.0f, 10.0f };
	m_Hero.AddNativeScript<HeroScript>();
	auto& ccc = m_Hero.AddComponent<Engine::Component::Physics::CharacterControllerComponent>(2.0f, 1.2f, 0.3f);
	Engine::System::Util::Activate(ccc);

	// Camera
	Engine::Entity camera = m_Scene->CreateEntity("Camera");
	camera.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { 0.0f, 0.9f, 0.0f };
	camera.AddComponent<Engine::Component::Renderer::CameraComponent>();
	camera.AddComponent<Engine::Component::Core::ParentComponent>(m_Hero);
	m_Scene->SetMainCamera(camera);

	// Skybox
	m_Skybox = Engine::CreateRef<Engine::Skybox>();

	// Seed
	srand(420);

	// Add objects to the Scene
	{
		Engine::Entity entity;

		// right hand
		entity = m_Scene->CreateEntity("Right Hand");
		entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { 0.25f, 0.75f, -0.35f };
		entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = { 1.0f, -2.0f, 1.0f };
		entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };
		entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("SkinMaterial");
		entity.AddComponent<Engine::Component::Renderer::MeshComponent>("Hand");
		entity.AddComponent<Engine::Component::Core::ParentComponent>(m_Hero);

		// left hand
		entity = m_Scene->CreateEntity("Left Hand");
		entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { -0.25f, 0.75f, -0.35f };
		entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = { -1.0f, 2.0f, -1.0f };
		entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { -1.0f, -1.0f, -1.0f };
		entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("SkinMaterial");
		entity.AddComponent<Engine::Component::Renderer::MeshComponent>("Hand");
		entity.AddComponent<Engine::Component::Core::ParentComponent>(m_Hero);

		// Background Sound
		entity = m_Scene->CreateEntity("Background Sound");
		entity.AddComponent<Engine::Component::Audio::Sound2DComponent>(Engine::SoundLibrary::Get("Forest"), true);

		// Monster
		if(true) {
			// Big Monsters
			for (size_t i = 0; i < 2; i++)
			{
				entity = m_Scene->CreateEntity("Monster [Big]");
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
				entity = m_Scene->CreateEntity("Monster [Small]");
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

		// House 1
		{
			entity = m_Scene->CreateEntity("House");
			std::string mesh = "House";
			glm::vec3 t{ 20.0f, 0.0f, 0.0f };
			glm::vec3 r{ 0.0f, -1.5f, 0.0f };
			glm::vec3 s{ 1.5f, 1.5f, 1.5f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("HouseMaterial");
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			entity.AddComponent<Engine::Component::Physics::StaticColliderComponent>();
		}
		// House 2
		{
			entity = m_Scene->CreateEntity("House");
			std::string mesh = "House";
			glm::vec3 t{ -20.0f, 0.0f, -20.0f };
			glm::vec3 r{ 0.0f, 0.5f, 0.0f };
			glm::vec3 s{ 1.5f, 1.5f, 1.5f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("HouseMaterial");
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			entity.AddComponent<Engine::Component::Physics::StaticColliderComponent>();
		}

		// Cave
		{
			entity = m_Scene->CreateEntity("Cave");
			std::string mesh = "Cave";
			glm::vec3 t{ -40.0f, 0.0f, -40.0f };
			glm::vec3 r{ 0.0f, -1.2f, 0.0f };
			glm::vec3 s{ 1.0f, 1.0f, 1.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("CaveMaterial");
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			entity.AddComponent<Engine::Component::Physics::StaticColliderComponent>();
		}

		// Pedestal
		{
			entity = m_Scene->CreateEntity("Pedestal");
			std::string mesh = "Pedestal";
			glm::vec3 t{ 0.0f, -0.2f, 0.0f };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 3.0f, 3.0f, 3.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("PedestalMaterial");
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			entity.AddComponent<Engine::Component::Physics::StaticColliderComponent>();
		}

		// Terrain
		{
			entity = m_Scene->CreateEntity("Terrain");
			std::string mesh = "Terrain";
			glm::vec3 t{ 0.0f, -0.45f, 0.0f };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.5f, 1.5f, 1.5f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("TerrainMaterial");
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			entity.AddComponent<Engine::Component::Physics::StaticColliderComponent>();
		}

		// Forest
		for (size_t i = 0; i < 100; i++)
		{
			entity = m_Scene->CreateEntity("Spruce");
			std::string mesh = ("Spruce_" + std::to_string((rand() % 5) + 1));
			glm::vec3 t{ (rand() % 100) - 50, -1.0f, (rand() % 100) - 50 };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.0f, 1.0f, 1.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("ForestMaterial");
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			entity.AddComponent<Engine::Component::Physics::StaticColliderComponent>();
		}
		for (size_t i = 0; i < 10; i++)
		{
			entity = m_Scene->CreateEntity("Stump");
			std::string mesh = ("Stump_" + std::to_string((rand() % 2) + 1));
			glm::vec3 t{ (rand() % 100) - 50, -0.1f, (rand() % 100) - 50 };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.0f, 1.0f, 1.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("ForestMaterial");
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			entity.AddComponent<Engine::Component::Physics::StaticColliderComponent>();
		}
		for (size_t i = 0; i < 10; i++)
		{
			entity = m_Scene->CreateEntity("Fallen Tree");
			std::string mesh = ("Fallen_Tree_" + std::to_string((rand() % 2) + 1));
			glm::vec3 t{ (rand() % 100) - 50, 0.2f, (rand() % 100) - 50 };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 0.5f, 0.5f, 0.5f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("ForestMaterial");
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			entity.AddComponent<Engine::Component::Physics::StaticColliderComponent>();
		}
		for (size_t i = 0; i < 30; i++)
		{
			entity = m_Scene->CreateEntity("Rock");
			std::string mesh = ("Rock_" + std::to_string((rand() % 1) + 1));
			glm::vec3 t{ (rand() % 100) - 50, 0.2f, (rand() % 100) - 50 };
			glm::vec3 r{ 0.0f, 0.2f, 0.0f };
			glm::vec3 s{ 1.0f, 1.0f, 1.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("ForestMaterial");
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);

			entity.AddComponent<Engine::Component::Physics::StaticColliderComponent>();
		}
		// Clouds
		for (size_t i = 0; i < 30; i++)
		{
			entity = m_Scene->CreateEntity("Clouds");
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { (rand() % 200) - 100, 60.0f, (rand() % 200) - 100 };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { 3.0f, 3.0f, 3.0f };
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>("CloudMaterial");
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>("Cloud_" + std::to_string((rand() % 3) + 1));
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

