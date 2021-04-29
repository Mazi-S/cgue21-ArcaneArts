#include "PhysicsTest.h"

using namespace Engine;

PhysicsTestLayer::PhysicsTestLayer()
	: Layer("Physics")
{
}

void PhysicsTestLayer::OnAttach()
{
	// Init here
	OpenGL::API::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	// Load Meshes
	{
		MeshLibrary::Load("Cube", "assets/objects/cube.obj");
		MeshLibrary::Load("Tree", "assets/objects/oak_2.obj");
		MeshLibrary::Load("Sphere", "assets/objects/sphere.obj");
		MeshLibrary::Load("Terrain", "assets/objects/terrain.obj");
	}

	// Load Shaders
	{
		ShaderLibrary::Load("TextureShader", "assets/shaders/Texture.glsl");
		ShaderLibrary::Load("ColorShader", "assets/shaders/FlatColor.glsl");
	}

	// Load Textures
	{
		TextureLibrary::LoadTexture2D("Bricks", "assets/textures/Bricks.jpg");
		TextureLibrary::LoadTexture2D("WoodFloor", "assets/textures/WoodFloor.jpg");
	}

	// Create Materials
	{
		}

	// Create Scene
	m_Scene = CreateRef<Scene>();

	// Light
	auto directionalLight = m_Scene->CreateEntity();
	directionalLight.AddComponent<Component::Renderer::DirectionalLightComponent>(glm::vec3{ 0.6f, -1.0f, 0.8f }, glm::vec3{ 0.8f, 0.8f, 0.8f });

	// Character
	m_Character = m_Scene->CreateEntity();
	m_Character.GetComponent<Component::Core::TransformComponent>().Translation = { 0.0f, 5.0f, 0.0f };
	auto& ccc = m_Character.AddComponent<Component::Physics::CharacterControllerComponent>(2.0f, 1.2f, 0.3f);
	System::Util::Activate(ccc);

	// Camera
	Entity camera = m_Scene->CreateEntity();
	camera.GetComponent<Component::Core::TransformComponent>().Translation = { 0.0f, 0.9f, 0.0f };
	camera.AddComponent<Component::Renderer::CameraComponent>();
	camera.AddComponent<Component::Core::ParentComponent>(m_Character);
	m_Scene->SetMainCamera(camera);

	InitScene();
}

void PhysicsTestLayer::OnDetach()
{
}

void PhysicsTestLayer::OnUpdate(Timestep ts)
{
	// Update here
	m_Scene->OnUpdate(ts);

	// Render here
	OpenGL::API::Clear();
	m_Scene->OnRender();
}

void PhysicsTestLayer::OnEvent(Event& event)
{
	m_Scene->OnEvent(event);

	EventHandler eventHandler(event);
	eventHandler.Handle<KeyPressedEvent>(EG_BIND_EVENT_FN(PhysicsTestLayer::OnKeyPressed));
}

bool PhysicsTestLayer::OnKeyPressed(KeyPressedEvent& event)
{
	if (event.GetKeyCode() == Key::Escape)
	{
		System::Util::Deactivate(m_Character.GetComponent<Component::Physics::CharacterControllerComponent>());
	}


	if (event.GetKeyCode() == Key::F1)
	{
		System::Util::Activate(m_Character.GetComponent<Component::Physics::CharacterControllerComponent>());
	}
	return false;
}

void PhysicsTestLayer::InitScene()
{
	Entity entity;

	auto& cubeMesh = MeshLibrary::Get("Cube");
	auto& sphereMesh = MeshLibrary::Get("Sphere");
	auto& treeMesh = MeshLibrary::Get("Tree");

	{
		glm::vec3 t{ 24.0f, 5.0f, 35.0f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 5.0f, 5.0f, 5.0f };

		entity = m_Scene->CreateEntity();
		entity.AddComponent<Component::Renderer::MaterialComponent>(MaterialLibrary::Get("GreenMaterial"));
		entity.AddComponent<Component::Renderer::MeshComponent>(sphereMesh);
		entity.GetComponent<Component::Core::TransformComponent>().Translation = t;
		entity.GetComponent<Component::Core::TransformComponent>().Rotation = r;
		entity.GetComponent<Component::Core::TransformComponent>().Scale = s;
	}

	{
		glm::vec3 t{ 24.0f, 5.0f, 25.0f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 3.0f, 10.0f, 0.5f };

		entity = m_Scene->CreateEntity();
		entity.AddComponent<Component::Renderer::MaterialComponent>(MaterialLibrary::Get("GreenMaterial"));
		entity.AddComponent<Component::Renderer::MeshComponent>(cubeMesh);
		entity.GetComponent<Component::Core::TransformComponent>().Translation = t;
		entity.GetComponent<Component::Core::TransformComponent>().Rotation = r;
		entity.GetComponent<Component::Core::TransformComponent>().Scale = s;
	}

	{
		glm::vec3 t{ 24.0f, 5.0f, -25.0f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 0.2f, 10.0f, 0.2f };

		entity = m_Scene->CreateEntity();
		entity.AddComponent<Component::Renderer::MaterialComponent>(MaterialLibrary::Get("GreenMaterial"));
		entity.AddComponent<Component::Renderer::MeshComponent>(cubeMesh);
		entity.GetComponent<Component::Core::TransformComponent>().Translation = t;
		entity.GetComponent<Component::Core::TransformComponent>().Rotation = r;
		entity.GetComponent<Component::Core::TransformComponent>().Scale = s;
	}

	// ground
	bool terrainGround = false;
	if(terrainGround)
	{
		entity = m_Scene->CreateEntity();

		auto mesh = MeshLibrary::Get("Terrain");
		entity.AddComponent<Component::Renderer::MaterialComponent>(MaterialLibrary::Get("GroundMaterial"));
		entity.AddComponent<Component::Renderer::MeshComponent>(mesh);
		glm::vec3 t{ 0.0f, 0.0f, 0.0f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 1.0f, 1.0f, 1.0f };
		entity.GetComponent<Component::Core::TransformComponent>().Translation = t;
		entity.GetComponent<Component::Core::TransformComponent>().Rotation = r;
		entity.GetComponent<Component::Core::TransformComponent>().Scale = s;
		auto actor = PhysicsAPI::CreateRigidStatic(t, r);
		auto shape = PhysicsAPI::CreateShape(mesh, s);
		entity.AddComponent<Component::Physics::RigidComponent>(actor);
		entity.AddComponent<Component::Physics::ShapeComponent>(shape);
	}
	else
	{
		entity = m_Scene->CreateEntity();

		entity.AddComponent<Component::Renderer::MaterialComponent>(MaterialLibrary::Get("GroundMaterial"));
		entity.AddComponent<Component::Renderer::MeshComponent>(cubeMesh);
		glm::vec3 t{ 0.0f, -0.4f, 0.0f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 200.0f, .8f, 200.0f };
		entity.GetComponent<Component::Core::TransformComponent>().Translation = t;
		entity.GetComponent<Component::Core::TransformComponent>().Rotation = r;
		entity.GetComponent<Component::Core::TransformComponent>().Scale = s;
		auto actor = PhysicsAPI::CreateRigidStatic(t, r);
		auto shape = PhysicsAPI::CreateShape(cubeMesh, s);
		entity.AddComponent<Component::Physics::RigidComponent>(actor);
		entity.AddComponent<Component::Physics::ShapeComponent>(shape);
	}

	{
		entity = m_Scene->CreateEntity();
		auto& mesh = MeshLibrary::Get("Sphere");
		entity.AddComponent<Component::Renderer::MaterialComponent>(MaterialLibrary::Get("GreenMaterial"));
		entity.AddComponent<Component::Renderer::MeshComponent>(mesh);

		glm::vec3 t{ 0.0f, 3.0f, -5.0f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 1.0f, 1.0f, 1.0f };
		entity.GetComponent<Component::Core::TransformComponent>().Translation = t;
		entity.GetComponent<Component::Core::TransformComponent>().Rotation = r;
		entity.GetComponent<Component::Core::TransformComponent>().Scale = s;
		auto actor = PhysicsAPI::CreateRigidDynamic(t, r);
		auto shape = PhysicsAPI::CreateSphereShape(1.0f);
		entity.AddComponent<Component::Physics::RigidDynamicComponent>(actor);
		entity.AddComponent<Component::Physics::ShapeComponent>(shape);
	}

	// stairs
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i <= 15; i++)
		{
			glm::vec3 t{ -10.0f - 5.0f * j, (0.2f + 0.1f * j) * i, 1.2f * i };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 2.0f, 0.1f + 0.05f * j, 2.0f };

			entity = m_Scene->CreateEntity();
			entity.AddComponent<Component::Renderer::MaterialComponent>(MaterialLibrary::Get("BricksMaterial"));
			entity.AddComponent<Component::Renderer::MeshComponent>(cubeMesh);
			entity.GetComponent<Component::Core::TransformComponent>().Translation = t;
			entity.GetComponent<Component::Core::TransformComponent>().Rotation = r;
			entity.GetComponent<Component::Core::TransformComponent>().Scale = s;
			auto actor = PhysicsAPI::CreateRigidStatic(t, r);
			auto shape = PhysicsAPI::CreateShape(cubeMesh, s);
			entity.AddComponent<Component::Physics::RigidComponent>(actor);
			entity.AddComponent<Component::Physics::ShapeComponent>(shape);
		}
	}

	for (int i = 1; i <= 10; i++)
	{
		glm::vec3 t{ 10.0f + 7.0f * i, -1.0f + 0.1f * i, .7f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 2.0f, 1.0f, 2.0f };

		entity = m_Scene->CreateEntity();
		entity.AddComponent<Component::Renderer::MaterialComponent>(MaterialLibrary::Get("BricksMaterial"));
		entity.AddComponent<Component::Renderer::MeshComponent>(cubeMesh);
		entity.GetComponent<Component::Core::TransformComponent>().Translation = t;
		entity.GetComponent<Component::Core::TransformComponent>().Rotation = r;
		entity.GetComponent<Component::Core::TransformComponent>().Scale = s;
		auto actor = PhysicsAPI::CreateRigidStatic(t, r);
		auto shape = PhysicsAPI::CreateShape(cubeMesh, s);
		entity.AddComponent<Component::Physics::RigidComponent>(actor);
		entity.AddComponent<Component::Physics::ShapeComponent>(shape);
	}

	for (int i = 0; i <= 6; i++)
	{
		glm::vec3 t{ 10.0f + 7.0f * i, 1.5f + 0.2f * i, 18.0f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 2.0f, 0.5f, 2.0f };

		entity = m_Scene->CreateEntity();
		entity.AddComponent<Component::Renderer::MaterialComponent>(MaterialLibrary::Get("BricksMaterial"));
		entity.AddComponent<Component::Renderer::MeshComponent>(cubeMesh);
		entity.GetComponent<Component::Core::TransformComponent>().Translation = t;
		entity.GetComponent<Component::Core::TransformComponent>().Rotation = r;
		entity.GetComponent<Component::Core::TransformComponent>().Scale = s;
		auto actor = PhysicsAPI::CreateRigidStatic(t, r);
		auto shape = PhysicsAPI::CreateShape(cubeMesh, s);
		entity.AddComponent<Component::Physics::RigidComponent>(actor);
		entity.AddComponent<Component::Physics::ShapeComponent>(shape);
	}

	for (int i = 1; i <= 8; i++)
	{
		glm::vec3 t{ -25.0f - 10 * i , 0.0f, 0.7f };
		glm::vec3 r{ glm::radians(-5.0f * i), 0.0f, 0.0f};
		glm::vec3 s{ 2.0f, 0.5f, 15.0f };
	
		entity = m_Scene->CreateEntity();
		entity.AddComponent<Component::Renderer::MaterialComponent>(MaterialLibrary::Get("BricksMaterial"));
		entity.AddComponent<Component::Renderer::MeshComponent>(cubeMesh);
		entity.GetComponent<Component::Core::TransformComponent>().Translation = t;
		entity.GetComponent<Component::Core::TransformComponent>().Rotation = r;
		entity.GetComponent<Component::Core::TransformComponent>().Scale = s;
		auto actor = PhysicsAPI::CreateRigidStatic(t, r);
		auto shape = PhysicsAPI::CreateShape(cubeMesh, s);
		entity.AddComponent<Component::Physics::RigidComponent>(actor);
		entity.AddComponent<Component::Physics::ShapeComponent>(shape);
	}
}
