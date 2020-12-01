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

	// Load Mesh: Cube
	Engine::Ref<Engine::VertexArray> cubeVA = Engine::VertexArray::Create();

	std::vector<float> vertices;
	std::vector<uint32_t> indices;

	loadOBJ("assets/objects/cube.obj", vertices, indices);
	
	Engine::Ref<Engine::VertexBuffer> cubeVB = Engine::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
	cubeVB->SetLayout({
		{ Engine::ShaderDataType::Float3, "a_Position" },
		{ Engine::ShaderDataType::Float2, "a_TexCoord" },
		{ Engine::ShaderDataType::Float3, "a_Normals" }
	});

	Engine::Ref<Engine::IndexBuffer> cubeIB = Engine::IndexBuffer::Create(indices.data(), indices.size());
	 
	cubeVA->AddVertexBuffer(cubeVB);
	cubeVA->SetIndexBuffer(cubeIB);

	// Load Mesh: m4a1
	Engine::Ref<Engine::VertexArray> m4a1VA = Engine::VertexArray::Create();

	vertices.clear();
	indices.clear();

	loadOBJ("assets/objects/m4a1.obj", vertices, indices);

	Engine::Ref<Engine::VertexBuffer> m4a1VB = Engine::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
	m4a1VB->SetLayout({
		{ Engine::ShaderDataType::Float3, "a_Position" },
		{ Engine::ShaderDataType::Float2, "a_TexCoord" },
		{ Engine::ShaderDataType::Float3, "a_Normals" }
	});

	Engine::Ref<Engine::IndexBuffer> m4a1IB = Engine::IndexBuffer::Create(indices.data(), indices.size());

	m4a1VA->AddVertexBuffer(m4a1VB);
	m4a1VA->SetIndexBuffer(m4a1IB);

	// Load Mesh: Sphere (MagicBall)
	m_MagicBallVA = Engine::VertexArray::Create();

	vertices.clear();
	indices.clear();

	loadOBJ("assets/objects/sphere.obj", vertices, indices);

	Engine::Ref<Engine::VertexBuffer> sphereVB = Engine::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
	sphereVB->SetLayout({
		{ Engine::ShaderDataType::Float3, "a_Position" },
		{ Engine::ShaderDataType::Float2, "a_TexCoord" },
		{ Engine::ShaderDataType::Float3, "a_Normals" }
		});

	Engine::Ref<Engine::IndexBuffer> sphereIB = Engine::IndexBuffer::Create(indices.data(), indices.size());

	m_MagicBallVA->AddVertexBuffer(sphereVB);
	m_MagicBallVA->SetIndexBuffer(sphereIB);

	// Load Shaders
	Engine::ShaderLibrary::Load("TextureShader", "assets/shaders/Texture.glsl");
	Engine::ShaderLibrary::Load("ColorShader", "assets/shaders/FlatColor.glsl");

	// Create Materials
	Engine::Ref<Engine::Material> redMaterial = Engine::Material::Create(Engine::MaterialProperties("RedMaterial", { 0.8f, 0.15f, 0.2f }), Engine::ShaderLibrary::Get("ColorShader"));
	Engine::Ref<Engine::Material> greenMaterial = Engine::Material::Create(Engine::MaterialProperties("GreenMaterial", { 0.2f, 0.8f, 0.15f }), Engine::ShaderLibrary::Get("ColorShader"));
	Engine::Ref<Engine::Material> bricksMaterial = Engine::Material::Create(Engine::MaterialProperties("BricksMaterial", { 1.0f, 1.0f, 1.0f }, "assets/textures/Bricks.jpg"), Engine::ShaderLibrary::Get("TextureShader"));
	Engine::Ref<Engine::Material> woodFloorMaterial = Engine::Material::Create(Engine::MaterialProperties("WoodFloorMaterial", { 1.0f, 1.0f, 1.0f }, "assets/textures/WoodFloor.jpg"), Engine::ShaderLibrary::Get("TextureShader"));
	
	// Create Scene
	m_Scene = Engine::CreateRef<Engine::Scene>();

	// Add objects to the Scene
	{
		Engine::Entity entity;
		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { -2.0f, 0.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.2f, 0.4f, 0.1f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };
		entity.AddComponent<Engine::MaterialComponent>(redMaterial);
		entity.AddComponent<Engine::MeshComponent>(cubeVA);

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { 2.0f, 0.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.5f, 0.4f, 0.2f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };
		entity.AddComponent<Engine::MaterialComponent>(greenMaterial);
		entity.AddComponent<Engine::MeshComponent>(cubeVA);

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { -2.0f, 0.0f, -3.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.2f, 0.2f, 0.7f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };
		entity.AddComponent<Engine::MaterialComponent>(bricksMaterial);
		entity.AddComponent<Engine::MeshComponent>(cubeVA);
		
		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { 2.0f, 0.0f, -3.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.7f, 0.5f, 0.9f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.5f, 0.5f, 0.5f };
		entity.AddComponent<Engine::MaterialComponent>(woodFloorMaterial);
		entity.AddComponent<Engine::MeshComponent>(cubeVA);

		entity = m_Scene->CreateEntity();
		entity.GetComponent<Engine::TransformComponent>().Translation = { -0.5f, 2.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Rotation = { 0.0f, 2.0f, 0.0f };
		entity.GetComponent<Engine::TransformComponent>().Scale = { 0.1f, 0.1f, 0.1f };
		entity.AddComponent<Engine::MaterialComponent>(redMaterial);
		entity.AddComponent<Engine::MeshComponent>(m4a1VA);
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
	// MaterialLibrary.Get("MagicBall");
	// MeshLibrary.Get("MagicBall");
	static Engine::Ref<Engine::Material> material = Engine::Material::Create(Engine::MaterialProperties("MagicBallMaterial",{ 0.8f, 0.8f, 0.8f }), Engine::ShaderLibrary::Get("ColorShader"));
	static Engine::Ref<Engine::VertexArray> vertexArray = m_MagicBallVA;

	Engine::Entity ball = m_Scene->CreateEntity("MagicBall");
	ball.AddComponent<Engine::MaterialComponent>(material);
	ball.AddComponent<Engine::MeshComponent>(vertexArray);

	// todo: get characters position and orientation
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 velocity = glm::vec3(1.0f, 1.0f, -50.0f);
	ball.AddNativeScript<MagicBall>(position, velocity);
}

