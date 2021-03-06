#include "egpch.h"
#include "Skybox.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

namespace Engine {

    Skybox::Skybox()
    {
        Engine::ShaderLibrary::Load("CubemapShader", "assets/shaders/Cubemap.glsl");
        Engine::ShaderLibrary::Load("SkyboxShader", "assets/shaders/Skybox.glsl");

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

        Engine::TextureLibrary::LoadCube("Skybox", faces);
    }

    Skybox::~Skybox()
    { }

    void Skybox::Draw(Camera camera) {
        auto skyboxShader = Engine::ShaderLibrary::Get("SkyboxShader");
        auto cubemapShader = Engine::ShaderLibrary::Get("CubemapShader");

        // Draw Knight
        auto& meshKnight = Engine::MeshLibrary::Get("Knight")->GetGlMesh();
        cubemapShader->Bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 4.5f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glm::mat4 view = glm::inverse(camera.Transform);
        glm::mat4 projection = camera.Projection;

        cubemapShader->SetInt("u_Skybox", 0);
        cubemapShader->SetMat4("u_Model", model);
        cubemapShader->SetMat4("u_View", view);
        cubemapShader->SetMat4("u_Projection", projection);

        meshKnight->GetSubmeshes()[0]->GetVertexArray()->Bind();
        Engine::TextureLibrary::GetCube("Skybox")->Bind();
        glDrawArrays(GL_TRIANGLES, 0, meshKnight->GetSubmeshes()[0]->GetVertexArray()->GetIndexBuffer()->GetCount());
        glBindVertexArray(0);

        // Draw Skybox
        auto& meshSkybox = Engine::MeshLibrary::Get("Cube")->GetGlMesh();
        glDepthFunc(GL_LEQUAL);
        skyboxShader->Bind();
        view = glm::mat4(glm::mat3(glm::inverse(camera.Transform)));
        skyboxShader->SetInt("u_Skybox", 0);
        skyboxShader->SetMat4("u_View", view);
        skyboxShader->SetMat4("u_Projection", projection);

        meshSkybox->GetSubmeshes()[0]->GetVertexArray()->Bind();
        Engine::TextureLibrary::GetCube("Skybox")->Bind();
        glDrawArrays(GL_TRIANGLES, 0, meshSkybox->GetSubmeshes()[0]->GetVertexArray()->GetIndexBuffer()->GetCount());
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

}