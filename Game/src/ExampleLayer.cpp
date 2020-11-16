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

	m_VA = Engine::VertexArray::Create();

	std::vector<float> vertices;
	std::vector<uint32_t> indices;

	bool res = loadOBJ("assets/objects/cube.obj", vertices, indices);
	
	Engine::Ref<Engine::VertexBuffer> vb = Engine::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
	vb->SetLayout({
		{ Engine::ShaderDataType::Float3, "a_Position" },
		{ Engine::ShaderDataType::Float2, "a_TexCoord" },
		{ Engine::ShaderDataType::Float3, "a_Normals" }
	});

	Engine::Ref<Engine::IndexBuffer> ib = Engine::IndexBuffer::Create(indices.data(), indices.size());
	 
	m_VA->AddVertexBuffer(vb);
	m_VA->SetIndexBuffer(ib);

	m_RedMaterial = Engine::CreateRef<ColorMaterial>(glm::vec4{ 0.8f, 0.2f, 0.15f, 1.0f });
	m_GreenMaterial = Engine::CreateRef<ColorMaterial>(glm::vec4{ 0.15f, 0.8f, 0.2f, 1.0f });
	m_BricksMaterial = Engine::CreateRef<TextureMaterial>("assets/textures/Bricks.jpg");
	m_WoodFloorMaterial = Engine::CreateRef<TextureMaterial>("assets/textures/WoodFloor.jpg");
	
	m_CameraController = Engine::CreateRef<Engine::CameraController>();
	m_Camera = Engine::CreateRef<Engine::SceneCamera>();
	m_Camera->SetViewportSize(Engine::Application::Get().GetWindow().GetWidth(), Engine::Application::Get().GetWindow().GetHeight());
	m_CameraController->temp_Camera = m_Camera;
}

void ExampleLayer::OnDetach()
{
	// Shutdown here
}

void ExampleLayer::OnUpdate(Engine::Timestep ts)
{
	// Update here
	// CameraController
	m_CameraController->OnUpdate(ts);

	// Render here
	Engine::OpenGL::API::Clear();

	// model matrix = translation * rotation * scale
	static glm::mat4 transform1
		= glm::translate(glm::mat4(1.0f), { -2.0f, 0.0f, 0.0f })
		* glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), { 0.5f, 0.7f, 0.2f })
		* glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5f } );

	static glm::mat4 transform2
		= glm::translate(glm::mat4(1.0f), { 2.0f, 0.0f, 0.0f })
		* glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), { 0.1f, 0.5f, 0.2f })
		* glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5f } );

	static glm::mat4 transform3
		= glm::translate(glm::mat4(1.0f), { -2.0f, 0.0f, -3.0f })
		* glm::rotate(glm::mat4(1.0f), glm::radians(64.0f), { 0.1f, 0.5f, 0.8f })
		* glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5f } );

	static glm::mat4 transform4
		= glm::translate(glm::mat4(1.0f), { 2.0f, 0.0f, -3.0f })
		* glm::rotate(glm::mat4(1.0f), glm::radians(75.0f), { 0.6f, 0.9f, 0.7f })
		* glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5f } );

	Engine::Renderer::BeginScene(m_Camera, m_CameraController->temp_Transform);
	Engine::Renderer::Submit(m_RedMaterial, m_VA, transform1);
	Engine::Renderer::Submit(m_GreenMaterial, m_VA, transform2);
	Engine::Renderer::Submit(m_BricksMaterial, m_VA, transform3);
	Engine::Renderer::Submit(m_WoodFloorMaterial, m_VA, transform4);
	Engine::Renderer::EndScene();
}

void ExampleLayer::OnEvent(Engine::Event& event)
{
	m_CameraController->OnEvent(event);
}


