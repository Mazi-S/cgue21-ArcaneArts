#include "ExampleLayer.h"

#include <Engine/Events/KeyEvent.h>
#include <Engine/Renderer/Texture.h>
#include <Engine/Renderer/ObjectLoader.h>

#include "Entities.h"

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
		Engine::MeshLibrary::Load("M4A1", "assets/objects/m4a1.obj");
		Engine::MeshLibrary::Load("Sphere", "assets/objects/sphere.obj");
	}

	// Load Shaders
	Engine::ShaderLibrary::Load("TextureShader", "assets/shaders/Texture.glsl");
	Engine::ShaderLibrary::Load("ColorShader", "assets/shaders/FlatColor.glsl");

	// Create Materials
	{
		auto redMaterial = Engine::Material::Create(Engine::MaterialProperties("RedMaterial", { 0.8f, 0.15f, 0.2f }), Engine::ShaderLibrary::Get("ColorShader"));
		auto greenMaterial = Engine::Material::Create(Engine::MaterialProperties("GreenMaterial", { 0.2f, 0.8f, 0.15f }), Engine::ShaderLibrary::Get("ColorShader"));
		auto bricksMaterial = Engine::Material::Create(Engine::MaterialProperties("BricksMaterial", { 1.0f, 1.0f, 1.0f }, "assets/textures/Bricks.jpg"), Engine::ShaderLibrary::Get("TextureShader"));
		auto woodFloorMaterial = Engine::Material::Create(Engine::MaterialProperties("WoodFloorMaterial", { 1.0f, 1.0f, 1.0f }, "assets/textures/WoodFloor.jpg"), Engine::ShaderLibrary::Get("TextureShader"));
		
		Engine::MaterialLibrary::Add(redMaterial);
		Engine::MaterialLibrary::Add(greenMaterial);
		Engine::MaterialLibrary::Add(bricksMaterial);
		Engine::MaterialLibrary::Add(woodFloorMaterial);

		Engine::MaterialLibrary::Add(Engine::Material::Create(Engine::MaterialProperties("MagicBall", { 0.7f, 0.7f, 0.7f }), Engine::ShaderLibrary::Get("ColorShader")));
		Engine::MaterialLibrary::Add(Engine::Material::Create(Engine::MaterialProperties("MagicBallRed", { 0.55f, 0.15f, 0.2f }), Engine::ShaderLibrary::Get("ColorShader")));
	}
	
	// Create Scene
	m_Scene = Engine::CreateRef<Engine::Scene>();

	// Hero
	auto hero = m_Scene->CreateHero();
	m_Scene->CreateMainCamera(hero);

	// Add objects to the Scene
	{
		Engine::Entity entity;
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

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { -0.0f, 2.0f, 2.0f };
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
	eventHandler.Handle<Engine::MouseButtonPressedEvent>(EG_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));
}

bool ExampleLayer::OnKeyPressed(Engine::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == Engine::Key::Escape)
		Engine::Application::Get().Close();

	if (event.GetKeyCode() == Engine::Key::D1)
		m_Scene->CreateMagicBall(true);

	if (event.GetKeyCode() == Engine::Key::D2)
		m_Scene->CreateMagicBall(false);
	return false;
}

bool ExampleLayer::OnMouseButtonPressed(Engine::MouseButtonPressedEvent& event)
{
	if (event.GetMouseButton() == Engine::Mouse::ButtonLeft)
	{
		CreateMagicBall();
		m_Scene->Throw(true);
	}

	if (event.GetMouseButton() == Engine::Mouse::ButtonRight)
		m_Scene->Throw(false);

	return false;
}

void ExampleLayer::CreateMagicBall()
{
	Engine::Entity ball = m_Scene->CreateEntity("MagicBall");
	ball.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall"));
	ball.AddComponent<Engine::MeshComponent>(Engine::MeshLibrary::Get("Sphere"));

	// todo: get characters position and orientation
	glm::mat4 characterTransform = glm::mat4(1.0f); // m_Scene->GetCharacterTransform();

	glm::vec3 position = { characterTransform[3][0], characterTransform[3][1], characterTransform[3][2] };
	glm::vec4 velocity = characterTransform * glm::vec4(0.0f, 0.0f, -50.0f, 0.0f);
	ball.AddNativeScript<MagicBall>(position, glm::vec3{ velocity.x, velocity.y, velocity.z });
}

