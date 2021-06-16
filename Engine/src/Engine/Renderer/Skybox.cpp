#include "egpch.h"
#include "Skybox.h"
#include "ShaderLibrary.h"
#include "Mesh.h"
#include "TextureLibrary.h"

#include "Platform/OpenGL/OpenGLAPI.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

namespace Engine {

	Skybox::Skybox()
	{
		m_SkyboxShader = CreateRef<OpenGL::GlShader>("SkyboxShader", "assets/shaders/Skybox.glsl");
		m_SkyboxShader->Bind();
		m_SkyboxShader->SetFloat("u_Brightness", Engine::Application::Get().Brightness());

		m_CubemapShader = CreateRef<OpenGL::GlShader>("CubemapShader", "assets/shaders/Cubemap.glsl");

		// Skybox Texture
		std::vector<std::string> faces
		{
			"assets/textures/skybox/right.jpg",
			"assets/textures/skybox/left.jpg",
			"assets/textures/skybox/bottom.jpg",
			"assets/textures/skybox/top.jpg",
			"assets/textures/skybox/front.jpg",
			"assets/textures/skybox/back.jpg",
		};

		m_SkyboxTex = CreateRef<OpenGL::GlCubeTexture>("Skybox", faces);
	}

	void Skybox::Draw(Camera camera)
	{
		// Draw Knight
		auto& meshKnight = Engine::MeshLibrary::Get("Knight")->GetGlMesh();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.5f, 2.7f, -4.0f));
		model = glm::rotate(model, -1.0f, glm::vec3{0,1,0});
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		glm::mat4 view = glm::inverse(camera.Transform);
		glm::mat4 projection = camera.Projection;

		m_CubemapShader->Bind();
		m_CubemapShader->SetInt("u_Skybox", 0);
		m_CubemapShader->SetMat4("u_Model", model);
		m_CubemapShader->SetMat4("u_View", view);
		m_CubemapShader->SetMat4("u_Projection", projection);

		meshKnight->GetSubmeshes()[0]->GetVertexArray()->Bind();
		m_SkyboxTex->Bind();
		glDrawArrays(GL_TRIANGLES, 0, meshKnight->GetSubmeshes()[0]->GetVertexArray()->GetIndexBuffer()->GetCount());

		// Draw Skybox
		OpenGL::API::CullFaces(OpenGL::Face::Front);
		auto& meshSkybox = Engine::MeshLibrary::Get("Cube")->GetGlMesh();

		view = glm::mat4(glm::mat3(glm::inverse(camera.Transform)));

		m_SkyboxShader->Bind();
		m_SkyboxShader->SetInt("u_Skybox", 0);
		m_SkyboxShader->SetMat4("u_View", view);
		m_SkyboxShader->SetMat4("u_Projection", projection);

		meshSkybox->GetSubmeshes()[0]->GetVertexArray()->Bind();
		m_SkyboxTex->Bind();
		glDrawArrays(GL_TRIANGLES, 0, meshSkybox->GetSubmeshes()[0]->GetVertexArray()->GetIndexBuffer()->GetCount());
		OpenGL::API::CullFaces();
	}

}