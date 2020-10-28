#include "ExampleLayer.h"

#include <iostream>
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

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	bool res = loadOBJ("assets/objects/cube.obj", vertices, uvs, normals);
	
	Engine::Ref<Engine::VertexBuffer> vb = Engine::VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(glm::vec3));
	vb->SetLayout({
		{ Engine::ShaderDataType::Float3, "a_Position" }
	});

	for (int i = 0; i < vertices.size(); i++) {
		std::cout << "(" << vertices[i].x <<  ", " << vertices[i].y << ", " << vertices[i].y << ")" << std::endl;
	}

	Engine::Ref<Engine::VertexBuffer> uvvb = Engine::VertexBuffer::Create(&uvs[0], uvs.size() * sizeof(glm::vec3));
	uvvb->SetLayout({
		{ Engine::ShaderDataType::Float2, "a_Texture" }
		});

	Engine::Ref<Engine::IndexBuffer> ib = Engine::IndexBuffer::Create(new unsigned int[vertices.size()], vertices.size());
	
	m_VA->AddVertexBuffer(vb);
	m_VA->AddVertexBuffer(uvvb);
	m_VA->SetIndexBuffer(ib);

	m_Shader = Engine::Shader::Create("assets/shaders/FlatColor.glsl");
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


	// Render here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Shader->Bind();
	m_Shader->SetFloat4("u_Color", color);
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
	// just for testing
	Engine::EventHandler eventHandler(event);
	eventHandler.Handle<Engine::KeyPressedEvent>(printKey);
}


