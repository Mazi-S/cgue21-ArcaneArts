#include "egpch.h"
#include "Skybox.h"
#include "Shader.h"
#include "Mesh.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

namespace Engine {

    unsigned int LoadCubeMap(std::vector<std::string> faces);
    unsigned int LoadTexture(char const* path);

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
        cubemapTexture = LoadCubeMap(faces);
    }

    Skybox::~Skybox()
    {
        glDeleteVertexArrays(1, &skyboxVAO);
        glDeleteBuffers(1, &skyboxVAO);
    }

    void Skybox::Draw(Camera camera) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto skyboxShader = Engine::ShaderLibrary::Get("SkyboxShader");
        auto cubemapShader = Engine::ShaderLibrary::Get("CubemapShader");
        cubemapShader->Bind();
        cubemapShader->SetInt("u_Skybox", 0);
        skyboxShader->Bind();
        skyboxShader->SetInt("u_Skybox", 0);

        // Draw Knight
        auto& meshKnight = Engine::MeshLibrary::Get("Knight")->GetGlMesh();
        cubemapShader->Bind();
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(2.0f, 0.0f, 3.0f));
        glm::mat4 view = glm::inverse(camera.Transform);
        glm::mat4 projection = camera.Projection;
        cubemapShader->SetMat4("u_Model", model);
        cubemapShader->SetMat4("u_View", view);
        cubemapShader->SetMat4("u_Projection", projection);

        meshKnight->GetSubmeshes()[0]->GetVertexArray()->Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, meshKnight->GetSubmeshes()[0]->GetVertexArray()->GetIndexBuffer()->GetCount());
        glBindVertexArray(0);

        // Draw Skybox
        auto& meshSkybox = Engine::MeshLibrary::Get("Cube")->GetGlMesh();
        glDepthFunc(GL_LEQUAL);
        skyboxShader->Bind();
        view = glm::mat4(glm::mat3(glm::inverse(camera.Transform)));
        skyboxShader->SetMat4("u_View", view);
        skyboxShader->SetMat4("u_Projection", projection);

        meshSkybox->GetSubmeshes()[0]->GetVertexArray()->Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, meshSkybox->GetSubmeshes()[0]->GetVertexArray()->GetIndexBuffer()->GetCount());
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

    unsigned int LoadCubeMap(std::vector<std::string> faces)
    {
        stbi_set_flip_vertically_on_load(1);
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrComponents;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }
}