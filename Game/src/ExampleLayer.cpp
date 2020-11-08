#include "ExampleLayer.h"

#include <Engine/Events/KeyEvent.h>
#include <Engine/Renderer/ObjectLoader.h>


#include <glad/glad.h>

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

	m_Shader = Engine::Shader::Create("assets/shaders/FlatColor.glsl");

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

	Engine::Renderer::BeginScene(m_Camera, m_CameraController->temp_Transform);
	Engine::Renderer::Submit(m_Shader, m_VA);
	Engine::Renderer::EndScene();
}

void ExampleLayer::OnEvent(Engine::Event& event)
{
	m_CameraController->OnEvent(event);
}


