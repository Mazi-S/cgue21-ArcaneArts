#include "ExampleLayer.h"

#include <Engine/Events/KeyEvent.h>


#include <glad/glad.h>

ExampleLayer::ExampleLayer()
	: Layer("Example")
{
}

void ExampleLayer::OnAttach()
{
	// Init here
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	m_VA = Engine::VertexArray::Create();
	
	float vertices[3 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	uint32_t indices[3] = { 0, 1, 2 };
	
	Engine::Ref<Engine::VertexBuffer> vb = Engine::VertexBuffer::Create(vertices, sizeof(vertices));
	vb->SetLayout({
		{ Engine::ShaderDataType::Float3, "a_Position" }
	});

	Engine::Ref<Engine::IndexBuffer> ib = Engine::IndexBuffer::Create(indices, 3);
	
	m_VA->AddVertexBuffer(vb);
	m_VA->SetIndexBuffer(ib);

	m_Shader = Engine::Shader::Create("assets/shaders/FlatColor.glsl");

	m_CameraController = Engine::CreateRef<Engine::CameraController>();
	m_Camera = Engine::CreateRef<Engine::SceneCamera>();
}

void ExampleLayer::OnDetach()
{
	// Shutdown here
}

void ExampleLayer::OnUpdate(Engine::Timestep ts)
{
	// Update here
	glm::vec4 color = { 0.2f, 0.2f, 0.2f , 1.0f };
	if (Engine::Input::IsKeyPressed(Engine::Key::R))
		color.r = 0.8f;
	if (Engine::Input::IsKeyPressed(Engine::Key::G))
		color.g = 0.8f;
	if (Engine::Input::IsKeyPressed(Engine::Key::B))
		color.b = 0.8f;

	// CameraController
	m_CameraController->OnUpdate(ts);

	// Render here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProjection = m_Camera->GetProjection() * glm::inverse(m_CameraController->temp_Transform);

	m_Shader->Bind();
	m_Shader->SetFloat4("u_Color", color);
	m_Shader->SetMat4("u_ViewProjection", viewProjection);
	m_VA->Bind();
	glDrawElements(GL_TRIANGLES, m_VA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

}

static bool printKey(Engine::KeyPressedEvent e)
{
	std::stringstream ss;
	ss << "KeyPressedEvent: " << e.GetKeyCode();
	LOG_INFO(ss.str());
	return false;
}

void ExampleLayer::OnEvent(Engine::Event& event)
{
	m_CameraController->OnEvent(event);
}


