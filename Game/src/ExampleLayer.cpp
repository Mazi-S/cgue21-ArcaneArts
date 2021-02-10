#include "ExampleLayer.h"

#include <Engine/Events/KeyEvent.h>
#include <Engine/Renderer/Texture.h>
#include <Engine/Util/ObjectLoader.h>

#include "Entities/Hero.h"
#include "Entities/MagicBall.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
}

void ExampleLayer::OnAttach()
{
	// Init here
	Engine::OpenGL::API::SetClearColor({ 0.0f, 0.3f, 0.5f, 1.0f });

	// Load Meshes
	{
		Engine::MeshLibrary::Load("Cube", "assets/objects/cube.obj");
		Engine::MeshLibrary::Load("Sphere", "assets/objects/sphere.obj");
		Engine::MeshLibrary::Load("Hand", "assets/objects/hand.obj");
		Engine::MeshLibrary::Load("Monster", "assets/objects/monster.obj");
		Engine::MeshLibrary::Load("House", "assets/objects/house.obj");
		Engine::MeshLibrary::Load("Terrain", "assets/objects/terrain.obj");

		// Forest
		Engine::MeshLibrary::Load("Log", "assets/objects/forest/log.obj");
		Engine::MeshLibrary::Load("Spruce_1", "assets/objects/forest/spruce_1.obj");
		Engine::MeshLibrary::Load("Spruce_2", "assets/objects/forest/spruce_2.obj");
		Engine::MeshLibrary::Load("Spruce_3", "assets/objects/forest/spruce_3.obj");
		Engine::MeshLibrary::Load("Spruce_Small_1", "assets/objects/forest/spruce_small_1.obj");
		Engine::MeshLibrary::Load("Spruce_Small_2", "assets/objects/forest/spruce_small_2.obj");
		Engine::MeshLibrary::Load("Oak_1", "assets/objects/forest/oak_1.obj");
		Engine::MeshLibrary::Load("Oak_2", "assets/objects/forest/oak_2.obj");
		Engine::MeshLibrary::Load("Fallen_Tree_1", "assets/objects/forest/fallen_tree_1.obj");
		Engine::MeshLibrary::Load("Fallen_Tree_2", "assets/objects/forest/fallen_tree_2.obj");
		Engine::MeshLibrary::Load("Stump_1", "assets/objects/forest/stump_1.obj");
		Engine::MeshLibrary::Load("Stump_2", "assets/objects/forest/stump_2.obj");
		Engine::MeshLibrary::Load("Stump_3", "assets/objects/forest/stump_3.obj");

		// Clouds
		Engine::MeshLibrary::Load("Cloud_1", "assets/objects/clouds/cloud_1.obj");
		Engine::MeshLibrary::Load("Cloud_2", "assets/objects/clouds/cloud_2.obj");
		Engine::MeshLibrary::Load("Cloud_3", "assets/objects/clouds/cloud_3.obj");
		Engine::MeshLibrary::Load("Cloud_4", "assets/objects/clouds/cloud_4.obj");

		// Rocks
		Engine::MeshLibrary::Load("Rock_1", "assets/objects/rocks/rock_1.obj");
		Engine::MeshLibrary::Load("Rock_2", "assets/objects/rocks/rock_2.obj");
		Engine::MeshLibrary::Load("Rock_3", "assets/objects/rocks/rock_3.obj");
		Engine::MeshLibrary::Load("Rock_4", "assets/objects/rocks/rock_4.obj");
		Engine::MeshLibrary::Load("Rock_5", "assets/objects/rocks/rock_5.obj");
	}

	// Load Shaders
	Engine::ShaderLibrary::Load("TextureShader", "assets/shaders/Texture.glsl");
	Engine::ShaderLibrary::Load("ColorShader", "assets/shaders/FlatColor.glsl");

	// Create Materials
	{
		// Colors
		auto redMaterial = Engine::Material::Create(Engine::MaterialProperties("RedMaterial", { 0.2f, 0.01f, 0.05f }, { 0.7f, 0.05f, 0.1f }, { 0.5f, 0.2f, 0.4f }, 2.0f), Engine::ShaderLibrary::Get("ColorShader"));
		auto greenMaterial = Engine::Material::Create(Engine::MaterialProperties("GreenMaterial", { 0.1f, 0.3f, 0.05f }), Engine::ShaderLibrary::Get("ColorShader"));
		auto skinMaterial = Engine::Material::Create(Engine::MaterialProperties("SkinMaterial", { 1.0f, 0.8f, 0.6f }), Engine::ShaderLibrary::Get("ColorShader"));
		auto cloudMaterial = Engine::Material::Create(Engine::MaterialProperties("CloudMaterial", { 0.56f, 0.77f, 1.0f }), Engine::ShaderLibrary::Get("ColorShader"));
		auto terrainMaterial = Engine::Material::Create(Engine::MaterialProperties("TerrainMaterial", { 0.26f, 0.33f, 0.0f }), Engine::ShaderLibrary::Get("ColorShader"));
		auto rockMaterial = Engine::Material::Create(Engine::MaterialProperties("RockMaterial", { 0.5f, 0.5f, 0.5f }), Engine::ShaderLibrary::Get("ColorShader"));

		// Textures
		auto bricksMaterial = Engine::Material::Create(Engine::MaterialProperties("BricksMaterial", { 0.1f, 0.1f, 0.1f }, { 0.6f, 0.6f, 0.6f }, { 0.2f, 0.2f, 0.2f }, 2.0f, "assets/textures/Bricks.jpg"), Engine::ShaderLibrary::Get("TextureShader"));
		auto woodFloorMaterial = Engine::Material::Create(Engine::MaterialProperties("WoodFloorMaterial", { 1.0f, 1.0f, 1.0f }, "assets/textures/WoodFloor.jpg"), Engine::ShaderLibrary::Get("TextureShader"));
		auto monsterMaterial = Engine::Material::Create(Engine::MaterialProperties("MonsterMaterial", { 0.5f, 0.5f, 0.5f }, "assets/textures/monster.png"), Engine::ShaderLibrary::Get("TextureShader"));
		auto houseMaterial = Engine::Material::Create(Engine::MaterialProperties("HouseMaterial", { 0.5f, 0.5f, 0.5f }, "assets/textures/house.png"), Engine::ShaderLibrary::Get("TextureShader"));
		auto forestMaterial = Engine::Material::Create(Engine::MaterialProperties("ForestMaterial", { 0.5f, 0.5f, 0.5f }, "assets/textures/forest.png"), Engine::ShaderLibrary::Get("TextureShader"));

		/*
		// Skybox
		std::vector<std::string> faces
		{
			"assets/textures/skybox/right.png",
			"assets/textures/skybox/left.png",
			"assets/textures/skybox/top.png",
			"assets/textures/skybox/bottom.png",
			"assets/textures/skybox/front.png",
			"assets/textures/skybox/back.png",
		};
		auto skyboxTexture = Engine::Texture::CreateCube(faces);
		auto skyboxShader = Engine::ShaderLibrary::Get("SkyboxShader");
		auto skyboxMaterial = Engine::TextureMaterial("SkyboxMaterial", { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f, skyboxTexture, skyboxShader);
		*/

		Engine::MaterialLibrary::Add(redMaterial);
		Engine::MaterialLibrary::Add(greenMaterial);
		Engine::MaterialLibrary::Add(bricksMaterial);
		Engine::MaterialLibrary::Add(woodFloorMaterial);
		Engine::MaterialLibrary::Add(skinMaterial);
		Engine::MaterialLibrary::Add(monsterMaterial);
		Engine::MaterialLibrary::Add(houseMaterial);
		Engine::MaterialLibrary::Add(forestMaterial);
		Engine::MaterialLibrary::Add(cloudMaterial);
		Engine::MaterialLibrary::Add(terrainMaterial);
		Engine::MaterialLibrary::Add(rockMaterial);
		// Engine::MaterialLibrary::Add(Engine::CreateRef<Engine::Material>(skyboxMaterial));

		Engine::MaterialLibrary::Add(Engine::Material::Create(Engine::MaterialProperties("MagicBall_Light", { 0.8f, 0.8f, 0.8f }), Engine::ShaderLibrary::Get("ColorShader")));
		Engine::MaterialLibrary::Add(Engine::Material::Create(Engine::MaterialProperties("MagicBall_Fire", { 0.5f, 0.05f, 0.1f }, { 0.5f, 0.05f, 0.1f }, { 0.5f, 0.35f, 0.4f }, 5.0f), Engine::ShaderLibrary::Get("ColorShader")));
		Engine::MaterialLibrary::Add(Engine::Material::Create(Engine::MaterialProperties("MagicBall_Water", { 0.1f, 0.05f, 0.5f }, { 0.1f, 0.05f, 0.5f }, { 0.4f, 0.35f, 0.5f }, 5.0f), Engine::ShaderLibrary::Get("ColorShader")));
		Engine::MaterialLibrary::Add(Engine::Material::Create(Engine::MaterialProperties("MagicBallRed", { 0.55f, 0.15f, 0.2f }), Engine::ShaderLibrary::Get("ColorShader")));
	}
	
	// Create Scene
	m_Scene = Engine::CreateRef<Engine::Scene>();

	// light
	auto directionalLight = m_Scene->CreateEntity();
	directionalLight.AddComponent<Engine::DirectionalLightComponent>(glm::vec3{ 0.0f, -1.0f, 0.3f }, glm::vec3{ 0.5f, 0.5f, 0.5f } );

	// Hero
	m_Hero = m_Scene->CreateEntity();
	m_Hero.AddNativeScript<Hero>();
	m_Hero.AddComponent<Engine::VelocityComponent>(glm::vec3{ 0.0f, -10.0f, 0.0f });
	auto& ccc = m_Hero.AddComponent<Engine::CharacterControllerComponent>();
	Engine::System::Util::Activate(ccc);

	m_Scene->CreateMainCamera(m_Hero);

	// Add objects to the Scene
	{
		Engine::Entity entity;
		/*
		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { -2.0f, 0.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.2f, 0.4f, 0.1f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("RedMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { 2.0f, 0.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.5f, 0.4f, 0.2f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("GreenMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { -2.0f, 0.0f, -3.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.2f, 0.2f, 0.7f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("BricksMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { 2.0f, 0.0f, -3.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.7f, 0.5f, 0.9f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("WoodFloorMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
		*/

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { -0.0f, 2.0f, 2.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("MonsterMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Monster"));

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { 0.3f, -0.15f, -0.5f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 1.0f, -2.0f, 1.0f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("SkinMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Hand"));
		entity.AddComponent<Engine::ParentComponent>(m_Hero);

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { -0.3f, -0.15f, -0.5f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { -1.0f, 2.0f, -1.0f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { -1.0f, -1.0f, -1.0f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("SkinMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Hand"));
		entity.AddComponent<Engine::ParentComponent>(m_Hero);

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { 20.0f, 0.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 1.5f, 1.5f, 1.5f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("HouseMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("House"));
		

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { 0.0f, -0.45f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 1.5f, 1.5f, 1.5f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("TerrainMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Terrain"));

		// Forest
		for (size_t i = 0; i < 50; i++)
		{
			entity = m_Scene->CreateEntity();
			entity.GetComponent<Engine::TransformComponent>().Translation = { (rand() % 100) - 50, 0.0f, (rand() % 100) - 50 };
			entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
			entity.GetComponent<Engine::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };
			entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("ForestMaterial"));
			entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Spruce_" + std::to_string((rand() % 3) + 1)));
		}
		for (size_t i = 0; i < 10; i++)
		{
			entity = m_Scene->CreateEntity();
			entity.GetComponent<Engine::TransformComponent>().Translation = { (rand() % 100) - 50, 0.0f, (rand() % 100) - 50 };
			entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
			entity.GetComponent<Engine::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };
			entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("ForestMaterial"));
			entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Oak_" + std::to_string((rand() % 2) + 1)));
		}
		for (size_t i = 0; i < 10; i++)
		{
			entity = m_Scene->CreateEntity();
			entity.GetComponent<Engine::TransformComponent>().Translation = { (rand() % 100) - 50, 0.0f, (rand() % 100) - 50 };
			entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
			entity.GetComponent<Engine::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };
			entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("ForestMaterial"));
			entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Spruce_Small_" + std::to_string((rand() % 2) + 1)));
		}
		for (size_t i = 0; i < 10; i++)
		{
			entity = m_Scene->CreateEntity();
			entity.GetComponent<Engine::TransformComponent>().Translation = { (rand() % 100) - 50, 0.0f, (rand() % 100) - 50 };
			entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
			entity.GetComponent<Engine::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };
			entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("ForestMaterial"));
			entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Stump_" + std::to_string((rand() % 3) + 1)));
		}
		for (size_t i = 0; i < 10; i++)
		{
			entity = m_Scene->CreateEntity();
			entity.GetComponent<Engine::TransformComponent>().Translation = { (rand() % 100) - 50, 0.0f, (rand() % 100) - 50 };
			entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
			entity.GetComponent<Engine::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };
			entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("ForestMaterial"));
			entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Fallen_Tree_" + std::to_string((rand() % 2) + 1)));
		}
		for (size_t i = 0; i < 30; i++)
		{
			entity = m_Scene->CreateEntity();
			entity.GetComponent<Engine::TransformComponent>().Translation = { (rand() % 100) - 50, -0.2f, (rand() % 100) - 50 };
			entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
			entity.GetComponent<Engine::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };
			entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("RockMaterial"));
			entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Rock_" + std::to_string((rand() % 5) + 1)));
		}

		// Clouds
		for (size_t i = 0; i < 30; i++)
		{
			entity = m_Scene->CreateEntity();
			entity.GetComponent<Engine::TransformComponent>().Translation = { (rand() % 200) - 100, 50.0f, (rand() % 200) - 100 };
			entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
			entity.GetComponent<Engine::TransformComponent>().Scale = { 4.0f, 4.0f, 4.0f };
			entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("CloudMaterial"));
			entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cloud_" + std::to_string((rand() % 4) + 1)));
		}

		/*
		glDepthFunc(GL_LEQUAL);
		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { 0.0f, 0.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 20.0f, 20.0f, 20.0f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("SkyboxMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
		glDepthFunc(GL_LEQUAL);
		*/
	}
}

void ExampleLayer::OnDetach()
{
	// Shutdown here
}

void ExampleLayer::OnUpdate(Engine::Timestep ts)
{
	// Update here
	m_Scene->OnUpdate(ts);

	// Render here
	Engine::OpenGL::API::Clear();
	m_Scene->OnRender();
}

void ExampleLayer::OnEvent(Engine::Event& event)
{
	m_Scene->OnEvent(event);

	Engine::EventHandler eventHandler(event);
	eventHandler.Handle<Engine::KeyPressedEvent>(EG_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
}

bool ExampleLayer::OnKeyPressed(Engine::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == Engine::Key::Escape)
	{
		Engine::System::Util::Deactivate(m_Hero.GetComponent<Engine::CharacterControllerComponent>());
		m_Hero.GetComponent<Engine::VelocityComponent>().Velocity = glm::vec3{ 0.0f, 0.0f, 0.0f };
	}


	if (event.GetKeyCode() == Engine::Key::F1)
	{
		Engine::System::Util::Activate(m_Hero.GetComponent<Engine::CharacterControllerComponent>());
		m_Hero.GetComponent<Engine::VelocityComponent>().Velocity = glm::vec3{ 0.0f, -10.0f, 0.0f };
	}
	return false;
}

