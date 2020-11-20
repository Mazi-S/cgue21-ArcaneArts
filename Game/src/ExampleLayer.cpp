#include "ExampleLayer.h"

#include <Engine/Events/KeyEvent.h>
#include <Engine/Renderer/Texture.h>
#include <Engine/Renderer/ObjectLoader.h>


#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Materials.h"

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

	// Create Materials
	Engine::Ref<Engine::Material> redMaterial = Engine::CreateRef<ColorMaterial>(glm::vec4{ 0.8f, 0.2f, 0.15f, 1.0f });
	Engine::Ref<Engine::Material> greenMaterial = Engine::CreateRef<ColorMaterial>(glm::vec4{ 0.15f, 0.8f, 0.2f, 1.0f });
	Engine::Ref<Engine::Material> bricksMaterial = Engine::CreateRef<TextureMaterial>("assets/textures/Bricks.jpg");
	Engine::Ref<Engine::Material> woodFloorMaterial = Engine::CreateRef<TextureMaterial>("assets/textures/WoodFloor.jpg");
	
	// Create Scene
	m_Scene = Engine::CreateRef<Engine::Scene>();

	// Add objects to the Scene
	{
		Engine::Entity entity;
		entity= m_Scene->CreateEntity();
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
}


