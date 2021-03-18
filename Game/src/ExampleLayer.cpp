#include "ExampleLayer.h"

#include <Engine/Events/KeyEvent.h>
#include <Engine/Util/ObjectLoader.h>

#include "Entities/Hero.h"
#include "Entities/MagicBall.h"
#include "Entities/Monster.h"

#include "Components/GameComponents.h"

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
		Engine::MeshLibrary::Load("Knight", "assets/objects/knight.obj");
		Engine::MeshLibrary::Load("Pedestal", "assets/objects/pedestal.obj");

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
	{
		Engine::ShaderLibrary::Load("TextureShader", "assets/shaders/Texture.glsl");
		Engine::ShaderLibrary::Load("ColorShader", "assets/shaders/FlatColor.glsl");
	}

	// Load Textures
	{
		Engine::TextureLibrary::LoadTexture2D("Bricks", "assets/textures/Bricks.jpg");
		Engine::TextureLibrary::LoadTexture2D("WoodFloor", "assets/textures/WoodFloor.jpg");
		Engine::TextureLibrary::LoadTexture2D("MonsterBig", "assets/textures/monster-big.png");
		Engine::TextureLibrary::LoadTexture2D("MonsterSmall", "assets/textures/monster-small.png");
		Engine::TextureLibrary::LoadTexture2D("House", "assets/textures/house.png");
		Engine::TextureLibrary::LoadTexture2D("Forest", "assets/textures/forest.png");
	}

	// Load Sounds
	{
		Engine::SoundLibrary::Load("FireballShoot", "assets/sounds/fireball-shoot.wav", 0.1f);
		Engine::SoundLibrary::Load("FireballCast", "assets/sounds/fireball-cast.wav", 0.3f);
		Engine::SoundLibrary::Load("Forest", "assets/sounds/forest.wav", 0.1f);
		Engine::SoundLibrary::Load("LightCast", "assets/sounds/light.wav", 0.4f);
		Engine::SoundLibrary::Load("Walk", "assets/sounds/walk.wav");
	}

	// Create Materials
	{
		// Colors
		Engine::MaterialLibrary::Create(Engine::MaterialProperties("SkinMaterial", { 0.2f, 0.15f, 0.1f }, { 0.4f, 0.2f, 0.2f }, { 0.1f, 0.1f, 0.1f }, 2.0f), Engine::ShaderLibrary::Get("ColorShader"));
		Engine::MaterialLibrary::Create(Engine::MaterialProperties("CloudMaterial", { 0.56f, 0.77f, 1.0f }, { 0.46f, 0.67f, 0.9f }, { 0.36f, 0.57f, 0.8f }), Engine::ShaderLibrary::Get("ColorShader"));
		Engine::MaterialLibrary::Create(Engine::MaterialProperties("TerrainMaterial", { 0.06f, 0.1f, 0.0f }, { 0.20f, 0.32f, 0.0f }, { 0.06f, 0.12f, 0.0f }), Engine::ShaderLibrary::Get("ColorShader"));
		Engine::MaterialLibrary::Create(Engine::MaterialProperties("RockMaterial", { 0.1f, 0.1f, 0.1f }, { 0.4f, 0.4f, 0.4f }, { 0.3f, 0.3f, 0.3f }), Engine::ShaderLibrary::Get("ColorShader"));
		Engine::MaterialLibrary::Create(Engine::MaterialProperties("PedestalMaterial", { 0.11f, 0.05f, 0.02f }, { 0.21f, 0.15f, 0.09f }, { 0.31f, 0.25f, 0.19f }), Engine::ShaderLibrary::Get("ColorShader"));

		// Textures
		Engine::MaterialLibrary::Create(Engine::MaterialProperties("MonsterBigMaterial", { 0.1f, 0.1f, 0.1f }, { 0.6f, 0.6f, 0.6f }, { 0.2f, 0.2f, 0.2f }, 2.0f), Engine::TextureLibrary::GetTexture2D("MonsterBig"), Engine::ShaderLibrary::Get("TextureShader"));
		Engine::MaterialLibrary::Create(Engine::MaterialProperties("MonsterSmallMaterial", { 0.1f, 0.1f, 0.1f }, { 0.6f, 0.6f, 0.6f }, { 0.2f, 0.2f, 0.2f }, 2.0f), Engine::TextureLibrary::GetTexture2D("MonsterSmall"), Engine::ShaderLibrary::Get("TextureShader"));
		Engine::MaterialLibrary::Create(Engine::MaterialProperties("HouseMaterial", { 0.1f, 0.1f, 0.1f }, { 0.6f, 0.6f, 0.6f }, { 0.2f, 0.2f, 0.2f }, 2.0f), Engine::TextureLibrary::GetTexture2D("House"), Engine::ShaderLibrary::Get("TextureShader"));
		Engine::MaterialLibrary::Create(Engine::MaterialProperties("ForestMaterial", { 0.1f, 0.1f, 0.1f }, { 0.45f, 0.45f, 0.45f }, { 0.25f, 0.25f, 0.25f }, 2.0f), Engine::TextureLibrary::GetTexture2D("Forest"), Engine::ShaderLibrary::Get("TextureShader"));

		Engine::MaterialLibrary::Create(Engine::MaterialProperties("MagicBall_Light", { 0.9f, 0.9f, 0.9f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }), Engine::ShaderLibrary::Get("ColorShader"));
		Engine::MaterialLibrary::Create(Engine::MaterialProperties("MagicBall_Fire", { 0.5f, 0.05f, 0.1f }, { 0.5f, 0.05f, 0.1f }, { 0.5f, 0.35f, 0.4f }, 5.0f), Engine::ShaderLibrary::Get("ColorShader"));
		Engine::MaterialLibrary::Create(Engine::MaterialProperties("MagicBall_Water", { 0.1f, 0.05f, 0.5f }, { 0.1f, 0.05f, 0.5f }, { 0.4f, 0.35f, 0.5f }, 5.0f), Engine::ShaderLibrary::Get("ColorShader"));
	}

	// Create Scene
	m_Scene = Engine::CreateRef<Engine::Scene>();

	// light
	auto directionalLight = m_Scene->CreateEntity();
	directionalLight.AddComponent<Engine::Component::Renderer::DirectionalLightComponent>(glm::vec3{ 0.0f, -1.0f, 0.8f }, glm::vec3{ 0.3f, 0.3f, 0.3f } );

	// Hero
	m_Hero = m_Scene->CreateEntity();
	m_Hero.AddComponent<Engine::Component::Audio::ListenerComponent>();
	m_Hero.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { 0.0f, 8.0f, 10.0f };
	m_Hero.AddNativeScript<Hero>();
	auto& ccc = m_Hero.AddComponent<Engine::Component::Physics::CharacterControllerComponent>(2.0f, 1.2f, 0.3f);
	Engine::System::Util::Activate(ccc);

	// Camera
	Engine::Entity camera = m_Scene->CreateEntity();
	camera.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { 0.0f, 0.9f, 0.0f };
	camera.AddComponent<Engine::Component::Renderer::CameraComponent>();
	camera.AddComponent<Engine::Component::Core::ParentComponent>(m_Hero);
	m_Scene->SetMainCamera(camera);

	// Skybox
	m_Skybox = Engine::CreateRef<Engine::Skybox>();

	// Seed
	srand(187);

	// Add objects to the Scene
	{
		Engine::Entity entity;

		// right hand
		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { 0.3f, 0.75f, -0.5f };
		entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = { 1.0f, -2.0f, 1.0f };
		entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };
		entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("SkinMaterial"));
		entity.AddComponent<Engine::Component::Renderer::MeshComponent>(Engine::MeshLibrary::Get("Hand"));
		entity.AddComponent<Engine::Component::Core::ParentComponent>(m_Hero);

		// left hand
		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { -0.3f, 0.75f, -0.5f };
		entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = { -1.0f, 2.0f, -1.0f };
		entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { -1.0f, -1.0f, -1.0f };
		entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("SkinMaterial"));
		entity.AddComponent<Engine::Component::Renderer::MeshComponent>(Engine::MeshLibrary::Get("Hand"));
		entity.AddComponent<Engine::Component::Core::ParentComponent>(m_Hero);

		// Background Sound
		entity = m_Scene->CreateEntity("Background Sound");
		entity.AddComponent<Engine::Component::Audio::Sound2DComponent>(Engine::SoundLibrary::Get("Forest"), true);

		// Monster
		{
			// Big Monsters
			for (size_t i = 0; i < 3; i++)
			{
				entity = m_Scene->CreateEntity();
				auto& mesh = Engine::MeshLibrary::Get("Monster");
				glm::vec3 t{ (rand() % 100) - 50, 5.5f, (rand() % 100) - 50 };
				glm::vec3 r{ 0.0f, 0.0f, 0.0f };
				glm::vec3 s{ 1.0f, 1.0f, 1.0f };
				entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
				entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
				entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
				entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("MonsterBigMaterial"));
				entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
				entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
				auto actor = Engine::PhysicsAPI::CreateRigidDynamic(t, r);
				auto shape = Engine::PhysicsAPI::CreateShape(mesh, s);
				entity.AddComponent<Engine::Component::Physics::RigidDynamicComponent>(actor);
				entity.AddComponent<Engine::Component::Physics::KinematicComponent>();
				entity.AddComponent<Engine::Component::Physics::ShapeComponent>(shape);
				entity.AddComponent<Engine::Component::Physics::KinematicMovementComponent>(glm::vec3{ 0,0,1 });
				entity.AddComponent<MonsterComponent>();
				entity.AddNativeScript<Monster>();
			}
			// Small Monsters
			for (size_t i = 0; i < 7; i++)
			{
				entity = m_Scene->CreateEntity();
				auto& mesh = Engine::MeshLibrary::Get("Monster");
				glm::vec3 t{ (rand() % 100) - 50, 5.5f, (rand() % 100) - 50 };
				glm::vec3 r{ 0.0f, 0.0f, 0.0f };
				glm::vec3 s{ 0.5f, 0.5f, 0.5f };
				entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
				entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
				entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
				entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("MonsterSmallMaterial"));
				entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
				entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
				auto actor = Engine::PhysicsAPI::CreateRigidDynamic(t, r);
				auto shape = Engine::PhysicsAPI::CreateShape(mesh, s);
				entity.AddComponent<Engine::Component::Physics::RigidDynamicComponent>(actor);
				entity.AddComponent<Engine::Component::Physics::KinematicComponent>();
				entity.AddComponent<Engine::Component::Physics::ShapeComponent>(shape);
				entity.AddComponent<Engine::Component::Physics::KinematicMovementComponent>(glm::vec3{ 0,0,1 });
				entity.AddComponent<MonsterComponent>(35.0f, 10.0f, 4.5f);
				entity.AddNativeScript<Monster>();
			}
		}

		// House
		{
			entity = m_Scene->CreateEntity();
			auto& mesh = Engine::MeshLibrary::Get("House");
			glm::vec3 t{ 20.0f, 0.0f, 0.0f };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.5f, 1.5f, 1.5f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("HouseMaterial"));
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			auto actor = Engine::PhysicsAPI::CreateRigidStatic(t, r);
			auto shape = Engine::PhysicsAPI::CreateShape(mesh, s);
			entity.AddComponent<Engine::Component::Physics::RigidComponent>(actor);
			entity.AddComponent<Engine::Component::Physics::ShapeComponent>(shape);
		}

		// Pedestal
		{
			entity = m_Scene->CreateEntity();
			auto& mesh = Engine::MeshLibrary::Get("Pedestal");
			glm::vec3 t{ 0.0f, 0.0f, 0.0f };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.0f, 1.0f, 1.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("PedestalMaterial"));
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			auto actor = Engine::PhysicsAPI::CreateRigidStatic(t, r);
			auto shape = Engine::PhysicsAPI::CreateShape(mesh, s);
			entity.AddComponent<Engine::Component::Physics::RigidComponent>(actor);
			entity.AddComponent<Engine::Component::Physics::ShapeComponent>(shape);
		}

		// Terrain
		{
			entity = m_Scene->CreateEntity();
			auto& mesh = Engine::MeshLibrary::Get("Terrain");
			glm::vec3 t{ 0.0f, -0.45f, 0.0f };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.5f, 1.5f, 1.5f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("TerrainMaterial"));
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			auto actor = Engine::PhysicsAPI::CreateRigidStatic(t, r);
			auto shape = Engine::PhysicsAPI::CreateShape(mesh, s);
			entity.AddComponent<Engine::Component::Physics::RigidComponent>(actor);
			entity.AddComponent<Engine::Component::Physics::ShapeComponent>(shape);
		}

		// Forest
		for (size_t i = 0; i < 50; i++)
		{
			entity = m_Scene->CreateEntity();
			auto& mesh = Engine::MeshLibrary::Get("Spruce_" + std::to_string((rand() % 3) + 1));
			glm::vec3 t{ (rand() % 100) - 50, -1.0f, (rand() % 100) - 50 };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.0f, 1.0f, 1.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("ForestMaterial"));
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			auto actor = Engine::PhysicsAPI::CreateRigidStatic(t, r);
			auto shape = Engine::PhysicsAPI::CreateShape(mesh, s);
			entity.AddComponent<Engine::Component::Physics::RigidComponent>(actor);
			entity.AddComponent<Engine::Component::Physics::ShapeComponent>(shape);
		}
		for (size_t i = 0; i < 10; i++)
		{
			entity = m_Scene->CreateEntity();
			auto& mesh = Engine::MeshLibrary::Get("Oak_" + std::to_string((rand() % 2) + 1));
			glm::vec3 t{ (rand() % 100) - 50, -1.0f, (rand() % 100) - 50 };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.0f, 1.0f, 1.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("ForestMaterial"));
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			auto actor = Engine::PhysicsAPI::CreateRigidStatic(t, r);
			auto shape = Engine::PhysicsAPI::CreateShape(mesh, s);
			entity.AddComponent<Engine::Component::Physics::RigidComponent>(actor);
			entity.AddComponent<Engine::Component::Physics::ShapeComponent>(shape);
		}
		for (size_t i = 0; i < 10; i++)
		{
			entity = m_Scene->CreateEntity();
			auto& mesh = Engine::MeshLibrary::Get("Spruce_Small_" + std::to_string((rand() % 2) + 1));
			glm::vec3 t{ (rand() % 100) - 50, -1.0f, (rand() % 100) - 50 };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.0f, 1.0f, 1.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("ForestMaterial"));
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			auto actor = Engine::PhysicsAPI::CreateRigidStatic(t, r);
			auto shape = Engine::PhysicsAPI::CreateShape(mesh, s);
			entity.AddComponent<Engine::Component::Physics::RigidComponent>(actor);
			entity.AddComponent<Engine::Component::Physics::ShapeComponent>(shape);
		}
		for (size_t i = 0; i < 10; i++)
		{
			entity = m_Scene->CreateEntity();
			auto& mesh = Engine::MeshLibrary::Get("Stump_" + std::to_string((rand() % 3) + 1));
			glm::vec3 t{ (rand() % 100) - 50, 0.0f, (rand() % 100) - 50 };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.0f, 1.0f, 1.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("ForestMaterial"));
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			auto actor = Engine::PhysicsAPI::CreateRigidStatic(t, r);
			auto shape = Engine::PhysicsAPI::CreateShape(mesh, s);
			entity.AddComponent<Engine::Component::Physics::RigidComponent>(actor);
			entity.AddComponent<Engine::Component::Physics::ShapeComponent>(shape);
		}
		for (size_t i = 0; i < 10; i++)
		{
			entity = m_Scene->CreateEntity();
			auto& mesh = Engine::MeshLibrary::Get("Fallen_Tree_" + std::to_string((rand() % 2) + 1));
			glm::vec3 t{ (rand() % 100) - 50, 0.0f, (rand() % 100) - 50 };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.0f, 1.0f, 1.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("ForestMaterial"));
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			auto actor = Engine::PhysicsAPI::CreateRigidStatic(t, r);
			auto shape = Engine::PhysicsAPI::CreateShape(mesh, s);
			entity.AddComponent<Engine::Component::Physics::RigidComponent>(actor);
			entity.AddComponent<Engine::Component::Physics::ShapeComponent>(shape);
		}
		for (size_t i = 0; i < 30; i++)
		{
			entity = m_Scene->CreateEntity();
			auto& mesh = Engine::MeshLibrary::Get("Rock_" + std::to_string((rand() % 5) + 1));
			glm::vec3 t{ (rand() % 100) - 50, -0.2f, (rand() % 100) - 50 };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 1.0f, 1.0f, 1.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("RockMaterial"));
			entity.AddComponent<Engine::Component::Renderer::ShadowComponent>();
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(mesh);
			auto actor = Engine::PhysicsAPI::CreateRigidStatic(t, r);
			auto shape = Engine::PhysicsAPI::CreateShape(mesh, s);
			entity.AddComponent<Engine::Component::Physics::RigidComponent>(actor);
			entity.AddComponent<Engine::Component::Physics::ShapeComponent>(shape);
		}

		// Clouds
		for (size_t i = 0; i < 30; i++)
		{
			entity = m_Scene->CreateEntity();
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Translation = { (rand() % 200) - 100, 50.0f, (rand() % 200) - 100 };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Rotation = { 0.0f, 0.0f, 0.0f };
			entity.GetComponent<Engine::Component::Core::TransformComponent>().Scale = { 4.0f, 4.0f, 4.0f };
			entity.AddComponent<Engine::Component::Renderer::MaterialComponent>(Engine::MaterialLibrary::Get("CloudMaterial"));
			entity.AddComponent<Engine::Component::Renderer::MeshComponent>(Engine::MeshLibrary::Get("Cloud_" + std::to_string((rand() % 4) + 1)));
		}
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

	// Render Skybox and CubeMap
	m_Skybox->Draw(m_Scene->GetCamera());

	// Render Scene
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

