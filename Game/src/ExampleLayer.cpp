#include "ExampleLayer.h"

#include <iostream>
#include <Engine/Events/KeyEvent.h>

#include <glad/glad.h>

ExampleLayer::ExampleLayer()
	: Layer("Example"), m_ShaderID(0)
{
}

static uint32_t CreateShader() {
	uint32_t rendererID = 0;

	std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec2 a_UV;
			out vec4 v_Color;
			void main() {
				gl_Position = vec4(a_Position, 1.0);
				v_Color = a_Color;
			}
		)";

	std::string fragmentSrc = R"(
			#version 330 core
			out vec4 color;
			in vec4 v_Color;
			void main() {
				color = v_Color;
			}
		)";


	// Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = vertexSrc.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(vertexShader);

		LOG_ERROR("{0}", infoLog.data());
		ASSERT(false, "Vertex shader compilation failure!");
		return 0;
	}

	// Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = fragmentSrc.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);

		LOG_ERROR("{0}", infoLog.data());
		ASSERT(false, "Fragment shader compilation failure!");
		return 0;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	rendererID = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(rendererID, vertexShader);
	glAttachShader(rendererID, fragmentShader);

	// Link our program
	glLinkProgram(rendererID);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(rendererID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(rendererID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(rendererID, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(rendererID);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		LOG_ERROR("{0}", infoLog.data());
		ASSERT(false, "Shader link failure!");
		return 0;
	}

	// Always detach shaders after a successful link.
	glDetachShader(rendererID, vertexShader);
	glDetachShader(rendererID, fragmentShader);

	return rendererID;
}

void ExampleLayer::OnAttach()
{
	// Init here
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	m_VA = Engine::VertexArray::Create();
	
	float vertices[3 * 9] = {
		-0.5f, -0.5f, 0.0f, 0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.9f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.9f, 0.9f, 0.0f, 1.0f, 0.0f, 1.0f
	};

	uint32_t indices[3] = { 0, 1, 2 };
	
	Engine::Ref<Engine::VertexBuffer> vb = Engine::VertexBuffer::Create(vertices, sizeof(vertices));
	vb->SetLayout({
		{ Engine::ShaderDataType::Float3, "a_Position" },
		{ Engine::ShaderDataType::Float4, "a_Color" },
		{ Engine::ShaderDataType::Float2, "a_UV" }
	});

	Engine::Ref<Engine::IndexBuffer> ib = Engine::IndexBuffer::Create(indices, 3);
	
	m_VA->AddVertexBuffer(vb);
	m_VA->SetIndexBuffer(ib);

	m_ShaderID = CreateShader();
}

void ExampleLayer::OnDetach()
{
	// Shutdown here
	glDeleteProgram(m_ShaderID);
}

void ExampleLayer::OnUpdate(Engine::Timestep ts)
{
	// Update here

	// Render here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_ShaderID);
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


