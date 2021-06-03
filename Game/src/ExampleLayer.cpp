
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

#include "Engine/Util/FileDialog.h"

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
	sceneSerializer.Deserialize("assets/scenes/test.scene");

	m_Scene->OnResume();

	Engine::Application::Get().GetWindow().HideCursor();

	// Skybox
	m_Skybox = Engine::CreateRef<Engine::Skybox>();

	// Seed
	srand(420);

	// Monster
	if (false) {
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
			entity.AddComponent<MonsterComponent>("Monster", "MonsterDying");
			entity.AddNativeScript<MonsterScript>();
			entity.AddComponent<Engine::Component::Core::Unserializable>();
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
			entity.AddComponent<MonsterComponent>("Monster", "MonsterDying", 35.0f, 10.0f, 4.5f, 30.0f, 2.5f);
			entity.AddNativeScript<MonsterScript>();
			entity.AddComponent<Engine::Component::Core::Unserializable>();
		}
	}

	// ImGui
	m_MaterialPanel = Engine::CreateScope<Engine::MaterialPanel>();
	m_ShaderPanel = Engine::CreateScope<Engine::ShaderPanel>();
	m_TexturePanel = Engine::CreateScope<Engine::Texture2DPanel>();
	m_MeshPanel = Engine::CreateScope<Engine::MeshPanel>();
	m_SceneHierarchyPanel = Engine::CreateScope<Engine::SceneHierarchyPanel>(std::bind(&ExampleLayer::NewScene, this), std::bind(&ExampleLayer::OpenScene, this), std::bind(&ExampleLayer::SaveScene, this));

	m_SceneHierarchyPanel->SetContext(m_Scene);
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
}

void ExampleLayer::OnEvent(Engine::Event& event)
{
	m_Scene->OnEvent(event);

	Engine::EventHandler eventHandler(event);
	eventHandler.HandleGameEvent<GameEndEvent>(EG_BIND_EVENT_FN(ExampleLayer::OnGameEnd));
	eventHandler.HandleGameEvent<MenuEvent>(EG_BIND_EVENT_FN(ExampleLayer::OnMenu));
}

bool ExampleLayer::OnGameEnd(GameEndEvent& event)
{
	Engine::Application::Get().Remove(this);
	return false;
}

bool ExampleLayer::OnMenu(MenuEvent& event)
{
	if (event.Open())
		m_Scene->OnPause();
	else
		m_Scene->OnResume();

	return false;
}

void ExampleLayer::SaveScene()
{
	std::string filepath = Engine::FileDialog::SaveFile("Scene (*.scene)\0*.scene\0");

	if (!filepath.empty())
	{
		Engine::SceneSerializer sceneSerializer(m_Scene);
		sceneSerializer.Serialize(filepath);
	}
}

void ExampleLayer::OpenScene()
{
	std::string filepath = Engine::FileDialog::OpenFile("Scene (*.scene)\0*.scene\0");

	if (!filepath.empty())
	{
		m_Scene = Engine::CreateRef<Engine::Scene>();
		m_SceneHierarchyPanel->SetContext(m_Scene);

		Engine::SceneSerializer sceneSerializer(m_Scene);
		sceneSerializer.Deserialize(filepath);
	}
}

void ExampleLayer::NewScene()
{
	m_Scene = Engine::CreateRef<Engine::Scene>();
	m_SceneHierarchyPanel->SetContext(m_Scene);
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

