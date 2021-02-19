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
	Engine::ShaderLibrary::Load("TextureShader", "assets/shaders/Texture.glsl");
	Engine::ShaderLibrary::Load("ColorShader", "assets/shaders/FlatColor.glsl");

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
	directionalLight.AddComponent<Engine::DirectionalLightComponent>(glm::vec3{ 0.0f, -1.0f, 0.3f }, glm::vec3{ 0.3f, 0.3f, 0.3f });

	// Character
	m_Character = m_Scene->CreateEntity();
	m_Character.GetComponent<Engine::TransformComponent>().Translation = { 0.0f, 2.0f, 0.0f };
	auto& ccc = m_Character.AddComponent<Engine::CharacterControllerComponent>();
	Engine::System::Util::Activate(ccc);

	// Camera
	Engine::Entity camera = m_Scene->CreateEntity();
	camera.GetComponent<Engine::TransformComponent>().Translation = { 0.0f, 1.0f, 0.0f };
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
}

void PhysicsTestLayer::InitScene()
{
	Engine::Entity entity;

	// ground
	{
		entity = m_Scene->CreateEntity();
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("GrayMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
		glm::vec3 t{ 0.0f, -1.0f, 0.0f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 200.0f, .2f, 200.0f };
		entity.GetComponent<Engine::TransformComponent>().Translation = t;
		entity.GetComponent<Engine::TransformComponent>().Rotation = r;
		entity.GetComponent<Engine::TransformComponent>().Scale = s;
		entity.AddComponent<Engine::RegidStaticComponent>(Engine::PhysicsAPI::CreateRigidStatic(Engine::MeshLibrary::Get("Tree"), t, r, s));
		entity.AddComponent<Engine::RegidStaticComponent>(Engine::PhysicsAPI::CreateRigidStatic(Engine::MeshLibrary::Get("Cube"), t, r, s));
	}

	// stairs
	for (int j = 0; j < 5; j++)
	{
		for (int i = 1; i <= 20; i++)
		{
			glm::vec3 t{ -10.0f - 5.0f * j, (0.2f + 0.1f * j) * i, .7f * i };
			glm::vec3 r{ 0.0f, 0.0f, 0.0f };
			glm::vec3 s{ 2.0f, 0.1f + 0.05f * j, 2.0f };

			entity = m_Scene->CreateEntity();
			entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("BricksMaterial"));
			entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
			entity.GetComponent<Engine::TransformComponent>().Translation = t;
			entity.GetComponent<Engine::TransformComponent>().Rotation = r;
			entity.GetComponent<Engine::TransformComponent>().Scale = s;
			entity.AddComponent<Engine::RegidStaticComponent>(Engine::PhysicsAPI::CreateRigidStatic(Engine::MeshLibrary::Get("Cube"), t, r, s));
		}
	}

	for (int i = 1; i <= 20; i++)
	{
		glm::vec3 t{ 10.0f + 7.0f * i, 0.2f + 0.1f * i, .7f };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 2.0f, 0.1f + 0.05f, 2.0f };

		entity = m_Scene->CreateEntity();
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("BricksMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
		entity.GetComponent<Engine::TransformComponent>().Translation = t;
		entity.GetComponent<Engine::TransformComponent>().Rotation = r;
		entity.GetComponent<Engine::TransformComponent>().Scale = s;
		entity.AddComponent<Engine::RegidStaticComponent>(Engine::PhysicsAPI::CreateRigidStatic(Engine::MeshLibrary::Get("Cube"), t, r, s));
	}

	for (int i = 1; i <= 6; i++)
	{
		glm::vec3 t{ 20.0f, 0.0f, 1.0f + 10 * i };
		glm::vec3 r{ 0.0f, 0.0f, 0.0f };
		glm::vec3 s{ 35.0f, 0.5f, 2.0f };

		entity = m_Scene->CreateEntity();
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("BricksMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
		entity.GetComponent<Engine::TransformComponent>().Translation = t;
		entity.GetComponent<Engine::TransformComponent>().Rotation = r;
		entity.GetComponent<Engine::TransformComponent>().Scale = s;
		entity.AddComponent<Engine::RegidStaticComponent>(Engine::PhysicsAPI::CreateRigidStatic(Engine::MeshLibrary::Get("Cube"), t, r, s));
	}

	entity = m_Scene->CreateEntity();
	entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("RedMaterial"));
	entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
	entity.AddComponent<Engine::RegidDynamicComponent>(Engine::PhysicsAPI::CreateRegidDynamic());

	entity = m_Scene->CreateEntity();
	entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("GreenMaterial"));
	entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
	entity.AddComponent<Engine::RegidDynamicComponent>(Engine::PhysicsAPI::CreateRegidDynamic());

	entity = m_Scene->CreateEntity();
	entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("BricksMaterial"));
	entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Cube"));
	entity.AddComponent<Engine::RegidDynamicComponent>(Engine::PhysicsAPI::CreateRegidDynamic());

	entity = m_Scene->CreateEntity();
	entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("GreenMaterial"));
	entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Tree"));
	glm::vec3 t{ 0.424f, -0.014f, 5.174f };
	glm::vec3 r{ 0, 2, 0 };
	glm::vec3 s{ .5, .5, .5 };
	entity.GetComponent<Engine::TransformComponent>().Translation = t;
	entity.GetComponent<Engine::TransformComponent>().Rotation = r;
	entity.GetComponent<Engine::TransformComponent>().Scale = s;
	entity.AddComponent<Engine::RegidStaticComponent>(Engine::PhysicsAPI::CreateRigidStatic(Engine::MeshLibrary::Get("Tree"), t, r, s));
}
