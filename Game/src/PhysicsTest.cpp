#include "PhysicsTest.h"

PhysicsTestLayer::PhysicsTestLayer()
	: Layer("Physics")
{
}

void PhysicsTestLayer::OnAttach()
{
	// Init here
	Engine::OpenGL::API::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	// Load Meshes
	{
		Engine::MeshLibrary::Load("Cube", "assets/objects/cube.obj");
		Engine::MeshLibrary::Load("Tree", "assets/objects/oak_2.obj");
		Engine::MeshLibrary::Load("Sphere", "assets/objects/sphere.obj");
	}

	// Load Shaders
	{
		Engine::ShaderLibrary::Load("TextureShader", "assets/shaders/Texture.glsl");
		Engine::ShaderLibrary::Load("ColorShader", "assets/shaders/FlatColor.glsl");
	}

	// Create Materials
	{
		auto grayMaterial = Engine::Material::Create(Engine::MaterialProperties("GrayMaterial", { 0.2f, 0.2f, 0.2f }, 2.0f), Engine::ShaderLibrary::Get("ColorShader"));
		auto redMaterial = Engine::Material::Create(Engine::MaterialProperties("RedMaterial", { 0.2f, 0.01f, 0.05f }, { 0.7f, 0.05f, 0.1f }, { 0.5f, 0.2f, 0.4f }, 2.0f), Engine::ShaderLibrary::Get("ColorShader"));
		auto greenMaterial = Engine::Material::Create(Engine::MaterialProperties("GreenMaterial", { 0.1f, 0.3f, 0.05f }), Engine::ShaderLibrary::Get("ColorShader"));
		auto bricksMaterial = Engine::Material::Create(Engine::MaterialProperties("BricksMaterial", { 0.1f, 0.1f, 0.1f }, { 0.6f, 0.6f, 0.6f }, { 0.2f, 0.2f, 0.2f }, 2.0f, "assets/textures/Bricks.jpg"), Engine::ShaderLibrary::Get("TextureShader"));
		auto woodFloorMaterial = Engine::Material::Create(Engine::MaterialProperties("WoodFloorMaterial", { 1.0f, 1.0f, 1.0f }, "assets/textures/WoodFloor.jpg"), Engine::ShaderLibrary::Get("TextureShader"));

		Engine::MaterialLibrary::Add(grayMaterial);
		Engine::MaterialLibrary::Add(redMaterial);
		Engine::MaterialLibrary::Add(greenMaterial);
		Engine::MaterialLibrary::Add(bricksMaterial);
		Engine::MaterialLibrary::Add(woodFloorMaterial);
	}

	// Create Scene
	m_Scene = Engine::CreateRef<Engine::Scene>();

	// Light
	auto directionalLight = m_Scene->CreateEntity();
	directionalLight.AddComponent<Engine::DirectionalLightComponent>(glm::vec3{ 0.0f, -1.0f, 0.3f }, glm::vec3{ 0.6f, 0.6f, 0.6f });

	// Character
	m_Character = m_Scene->CreateEntity();
	m_Character.GetComponent<Engine::TransformComponent>().Translation = { 0.0f, 2.0f, 0.0f };
	auto& ccc = m_Character.AddComponent<Engine::CharacterControllerComponent>(2.0f, 1.2f, 0.3f);
	Engine::System::Util::Activate(ccc);

	// Camera
	Engine::Entity camera = m_Scene->CreateEntity();
	camera.GetComponent<Engine::TransformComponent>().Translation = { 0.0f, 0.9f, 0.0f };
	camera.AddComponent<Engine::CameraComponent>();
	camera.AddComponent<Engine::ParentComponent>(m_Character);
	m_Scene->SetMainCamera(camera);

	InitScene();
}

void PhysicsTestLayer::OnDetach()
{
}

void PhysicsTestLayer::OnUpdate(Engine::Timestep ts)
{
	// Update here
	m_Scene->OnUpdate(ts);

	// Render here
	Engine::OpenGL::API::Clear();
	m_Scene->OnRender();
}

void PhysicsTestLayer::OnEvent(Engine::Event& event)
{
	m_Scene->OnEvent(event);

	Engine::EventHandler eventHandler(event);
	eventHandler.Handle<Engine::KeyPressedEvent>(EG_BIND_EVENT_FN(PhysicsTestLayer::OnKeyPressed));
}

bool PhysicsTestLayer::OnKeyPressed(Engine::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == Engine::Key::Escape)
	{
		Engine::System::Util::Deactivate(m_Character.GetComponent<Engine::CharacterControllerComponent>());
	}


	if (event.GetKeyCode() == Engine::Key::F1)
	{
		Engine::System::Util::Activate(m_Character.GetComponent<Engine::CharacterControllerComponent>());
	}
	return false;
}

void PhysicsTestLayer::InitScene()
{
	Engine::Entity entity;

	// ground
	{
		entity = m_Scene->CreateEntity();
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("GrayMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
		glm::vec3 t{ 0.0f, -0.1f, 0.0f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 200.0f, .1f, 200.0f };
		entity.GetComponent<Engine::TransformComponent>().Translation = t;
		entity.GetComponent<Engine::TransformComponent>().Rotation = r;
		entity.GetComponent<Engine::TransformComponent>().Scale = s;
		entity.AddComponent<Engine::RigidStaticComponent>(Engine::PhysicsAPI::CreateRigidStatic(Engine::MeshLibrary::Get("Cube"), t, r, s));
	}

	{
		entity = m_Scene->CreateEntity();
		auto& mesh = Engine::MeshLibrary::Get("Sphere");
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("GreenMaterial"));
		entity.AddComponent<Engine::MeshComponent>(mesh);

		glm::vec3 t{ 0.0f, 3.0f, -5.0f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 1.0f, 1.0f, 1.0f };
		entity.GetComponent<Engine::TransformComponent>().Translation = t;
		entity.GetComponent<Engine::TransformComponent>().Rotation = r;
		entity.GetComponent<Engine::TransformComponent>().Scale = s;
		entity.AddComponent<Engine::RigidDynamicComponent>(Engine::PhysicsAPI::CreateRigidDynamicSphere(t, 1.0f));
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
			entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("BricksMaterial"));
			entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
			entity.GetComponent<Engine::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::RigidStaticComponent>(Engine::PhysicsAPI::CreateRigidStatic(Engine::MeshLibrary::Get("Cube"), t, r, s));
		}
	}

	for (int i = 1; i <= 10; i++)
	{
		glm::vec3 t{ 10.0f + 7.0f * i, -1.0f + 0.1f * i, .7f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 2.0f, 1.0f, 2.0f };

		entity = m_Scene->CreateEntity();
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("BricksMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
		entity.GetComponent<Engine::TransformComponent>().Translation = t;
		entity.GetComponent<Engine::TransformComponent>().Rotation = r;
		entity.GetComponent<Engine::TransformComponent>().Scale = s;
		entity.AddComponent<Engine::RigidStaticComponent>(Engine::PhysicsAPI::CreateRigidStatic(Engine::MeshLibrary::Get("Cube"), t, r, s));
	}

	for (int i = 0; i <= 6; i++)
	{
		glm::vec3 t{ 10.0f + 7.0f * i, 1.5f + 0.2f * i, 18.0f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 2.0f, 0.5f, 2.0f };

		entity = m_Scene->CreateEntity();
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("BricksMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
		entity.GetComponent<Engine::TransformComponent>().Translation = t;
		entity.GetComponent<Engine::TransformComponent>().Rotation = r;
		entity.GetComponent<Engine::TransformComponent>().Scale = s;
		entity.AddComponent<Engine::RigidStaticComponent>(Engine::PhysicsAPI::CreateRigidStatic(Engine::MeshLibrary::Get("Cube"), t, r, s));
	}

	for (int i = 1; i <= 8; i++)
	{
		glm::vec3 t{ -25.0f - 10 * i , 0.0f, 0.7f };
		glm::vec3 r{ glm::radians(-5.0f * i), 0.0f, 0.0f};
		glm::vec3 s{ 2.0f, 0.5f, 15.0f };
	
		entity = m_Scene->CreateEntity();
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("BricksMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
		entity.GetComponent<Engine::TransformComponent>().Translation = t;
		entity.GetComponent<Engine::TransformComponent>().Rotation = r;
		entity.GetComponent<Engine::TransformComponent>().Scale = s;
		entity.AddComponent<Engine::RigidStaticComponent>(Engine::PhysicsAPI::CreateRigidStatic(Engine::MeshLibrary::Get("Cube"), t, r, s));
	}
}
