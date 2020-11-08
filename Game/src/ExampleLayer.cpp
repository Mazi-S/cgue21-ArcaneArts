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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

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


