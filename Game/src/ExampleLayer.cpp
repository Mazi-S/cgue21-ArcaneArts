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
	Engine::OpenGL::API::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	// Load Meshes
	{
		Engine::MeshLibrary::Load("Cube", "assets/objects/cube.obj");
		Engine::MeshLibrary::Load("Tree", "assets/objects/oak_2.obj");
		Engine::MeshLibrary::Load("M4A1", "assets/objects/m4a1.obj");
		Engine::MeshLibrary::Load("Sphere", "assets/objects/sphere.obj");
	}

	// Load Shaders
	Engine::ShaderLibrary::Load("TextureShader", "assets/shaders/Texture.glsl");
	Engine::ShaderLibrary::Load("ColorShader", "assets/shaders/FlatColor.glsl");

	// Create Materials
	{
		auto redMaterial = Engine::Material::Create(Engine::MaterialProperties("RedMaterial", { 0.2f, 0.01f, 0.05f }, { 0.7f, 0.05f, 0.1f }, { 0.5f, 0.2f, 0.4f }, 2.0f), Engine::ShaderLibrary::Get("ColorShader"));
		auto greenMaterial = Engine::Material::Create(Engine::MaterialProperties("GreenMaterial", { 0.1f, 0.3f, 0.05f }), Engine::ShaderLibrary::Get("ColorShader"));
		auto bricksMaterial = Engine::Material::Create(Engine::MaterialProperties("BricksMaterial", { 0.1f, 0.1f, 0.1f }, { 0.6f, 0.6f, 0.6f }, { 0.2f, 0.2f, 0.2f }, 2.0f, "assets/textures/Bricks.jpg"), Engine::ShaderLibrary::Get("TextureShader"));
		auto woodFloorMaterial = Engine::Material::Create(Engine::MaterialProperties("WoodFloorMaterial", { 1.0f, 1.0f, 1.0f }, "assets/textures/WoodFloor.jpg"), Engine::ShaderLibrary::Get("TextureShader"));
		
		Engine::MaterialLibrary::Add(redMaterial);
		Engine::MaterialLibrary::Add(greenMaterial);
		Engine::MaterialLibrary::Add(bricksMaterial);
		Engine::MaterialLibrary::Add(woodFloorMaterial);

		Engine::MaterialLibrary::Add(Engine::Material::Create(Engine::MaterialProperties("MagicBall_Light", { 0.8f, 0.8f, 0.8f }), Engine::ShaderLibrary::Get("ColorShader")));
		Engine::MaterialLibrary::Add(Engine::Material::Create(Engine::MaterialProperties("MagicBall_Fire", { 0.5f, 0.05f, 0.1f }, { 0.5f, 0.05f, 0.1f }, { 0.5f, 0.35f, 0.4f }, 5.0f), Engine::ShaderLibrary::Get("ColorShader")));
		Engine::MaterialLibrary::Add(Engine::Material::Create(Engine::MaterialProperties("MagicBall_Water", { 0.1f, 0.05f, 0.5f }, { 0.1f, 0.05f, 0.5f }, { 0.4f, 0.35f, 0.5f }, 5.0f), Engine::ShaderLibrary::Get("ColorShader")));
		Engine::MaterialLibrary::Add(Engine::Material::Create(Engine::MaterialProperties("MagicBallRed", { 0.55f, 0.15f, 0.2f }), Engine::ShaderLibrary::Get("ColorShader")));
	}
	
	// Create Scene
	m_Scene = Engine::CreateRef<Engine::Scene>();

	// light
	auto directionalLight = m_Scene->CreateEntity();
	directionalLight.AddComponent<Engine::DirectionalLightComponent>(glm::vec3{ 0.0f, -1.0f, 0.3f }, glm::vec3{ 0.3f, 0.3f, 0.3f } );

	// Hero
	m_Hero = m_Scene->CreateEntity();
	m_Hero.AddNativeScript<Hero>();
	auto& ccc = m_Hero.AddComponent<Engine::CharacterControllerComponent>();
	Engine::System::Util::Activate(ccc);

	m_Scene->CreateMainCamera(m_Hero);

	// Add objects to the Scene
	{
		Engine::Entity entity;
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
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("WoodFloorMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Tree"));
		entity.GetComponent<Engine::TransformComponent>().Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 r{ 0, 0, 0};
		glm::vec3 s{ 3, 1, 1};
		entity.GetComponent<Engine::TransformComponent>().Rotation = r;
		entity.GetComponent<Engine::TransformComponent>().Scale = s;
		entity.AddComponent<Engine::RegidStaticComponent>(Engine::PhysicsAPI::CreateRigidStatic(Engine::MeshLibrary::Get("Tree"), { 0.0f, 0.0f, 0.0f }, r, s));

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { 0.0f, 0.0f, -15.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 3.1f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.1f, 0.1f, 0.1f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("RedMaterial"));
		entity.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("M4A1"));
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
	}


	if (event.GetKeyCode() == Engine::Key::F1)
	{
		Engine::System::Util::Activate(m_Hero.GetComponent<Engine::CharacterControllerComponent>());
	}
	return false;
}

