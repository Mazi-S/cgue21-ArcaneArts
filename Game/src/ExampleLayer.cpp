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
	auto cube = Engine::LoadMesh("Cube", "assets/objects/cube.obj");
	auto m4a1 = Engine::LoadMesh("M4A1", "assets/objects/m4a1.obj");
	m_Sphere = Engine::LoadMesh("Sphere", "assets/objects/sphere.obj");

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

	// Add objects to the Scene
	{
		Engine::Entity entity;
		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { -2.0f, 0.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.2f, 0.4f, 0.1f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("RedMaterial"));
		entity.AddComponent<Engine::MeshComponent>(cube);

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { 2.0f, 0.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.5f, 0.4f, 0.2f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("GreenMaterial"));
		entity.AddComponent<Engine::MeshComponent>(cube);

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { -2.0f, 0.0f, -3.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.2f, 0.2f, 0.7f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("BricksMaterial"));
		entity.AddComponent<Engine::MeshComponent>(cube);
		
		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { 2.0f, 0.0f, -3.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.7f, 0.5f, 0.9f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("WoodFloorMaterial"));
		entity.AddComponent<Engine::MeshComponent>(cube);

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { -0.0f, 2.0f, 2.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 3.1f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.1f, 0.1f, 0.1f };
		entity.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("RedMaterial"));
		entity.AddComponent<Engine::MeshComponent>(m4a1);
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
	if (event.GetKeyCode() == Engine::Key::E)
		CreateMagicBall();

	return false;
}

void ExampleLayer::CreateMagicBall()
{
	// todo: MaterialLibrary and MeshLibrary
	// MeshLibrary.Get("MagicBall");

	Engine::Entity ball = m_Scene->CreateEntity("MagicBall");
	ball.AddComponent<Engine::MaterialComponent>(Engine::MaterialLibrary::Get("MagicBall"));
	ball.AddComponent<Engine::MeshComponent>(m_Sphere);

	// todo: get characters position and orientation
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 velocity = glm::vec3(1.0f, 1.0f, -50.0f);
	ball.AddNativeScript<MagicBall>(position, velocity);
}

