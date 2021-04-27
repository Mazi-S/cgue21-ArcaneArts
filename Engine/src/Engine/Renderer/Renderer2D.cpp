#include "egpch.h"
#include "Renderer2D.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLBufferLayout.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoord;
		float TextureIndex;
	};

	struct Renderer2DData
	{
		// Maximal per Draw Call
		static const uint32_t MaxQuads = 100;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<OpenGL::GlVertexArray> QuadVertexArray;
		Ref<OpenGL::GlVertexBuffer> QuadVertexBuffer;
		Ref<OpenGL::GlShader> BatchShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// Array: index(Slot) -> elem(Texture)
		std::array<Ref<OpenGL::GlTexture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 QuadVertexPositions[4];
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		LOG_INFO("Renderer2D::Init...");
		s_Data.QuadVertexArray = CreateRef<OpenGL::GlVertexArray>();

		s_Data.QuadVertexBuffer = CreateRef<OpenGL::GlVertexBuffer>(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ OpenGL::GlShaderDataType::Float3, "a_Position" },
			{ OpenGL::GlShaderDataType::Float4, "a_Color" },
			{ OpenGL::GlShaderDataType::Float2, "a_TexCoord" },
			{ OpenGL::GlShaderDataType::Float, "a_TexIndex" }
		});

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (int i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = 0 + offset;
			quadIndices[i + 1] = 1 + offset;
			quadIndices[i + 2] = 2 + offset;

			quadIndices[i + 3] = 2 + offset;
			quadIndices[i + 4] = 3 + offset;
			quadIndices[i + 5] = 0 + offset;

			offset += 4;
		}
		Ref<OpenGL::GlIndexBuffer> quadIB = CreateRef<OpenGL::GlIndexBuffer>(quadIndices, s_Data.MaxIndices);
		delete[] quadIndices;

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);

		// Shader
		s_Data.BatchShader = CreateRef<OpenGL::GlShader>("BatchShader", "assets/shaders/BatchShader2D.glsl");
		s_Data.BatchShader->Bind();
		s_Data.BatchShader->SetFloat("u_Brightness", Engine::Application::Get().Brightness2D());

		// Texture
		auto spec = OpenGL::Texture2DSpecification();
		spec.Width = 1;
		spec.Height = 1;
		spec.Internalformat = 0x8058;
		spec.Format = 0x1908;
		spec.Type = 0x1401;

		s_Data.TextureSlots[0] = CreateRef<OpenGL::GlTexture2D>("WhiteTexture", spec);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.TextureSlots[0]->SetData(&whiteTextureData, sizeof(uint32_t));

		int samplers[s_Data.MaxTextureSlots];
		for (int i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.BatchShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Quad Vertex Positions
		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		LOG_INFO("Renderer2D::Shutdown...");
		delete[] s_Data.QuadVertexBufferBase;

		s_Data.QuadVertexBufferBase = nullptr;
		s_Data.QuadVertexBufferPtr = nullptr;
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		glm::mat4 viewProjection = camera.Projection * glm::inverse(camera.Transform);

		s_Data.BatchShader->Bind();
		s_Data.BatchShader->SetMat4("u_ViewProjection", viewProjection);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		s_Data.QuadVertexArray->Bind();
		OpenGL::API::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		const size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		const glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			Flush();

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TextureCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<OpenGL::GlTexture2D>& texture, const glm::vec4 tintColor)
	{
		const size_t quadVertexCount = 4;
		const glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			Flush();

		float textureIndex = 0.0f; // White Texture

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i].get()->GetName() == texture.get()->GetName())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TextureCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
	}

}

