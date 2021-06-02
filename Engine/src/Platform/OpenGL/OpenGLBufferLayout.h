#pragma once
#include "Engine/Core/Base.h"

namespace Engine::OpenGL {

	enum class GlShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool,
		Struct
	};

	struct GlBufferElement
	{
		std::string Name;
		GlShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;

		GlBufferElement() = default;

		GlBufferElement(GlShaderDataType type, const std::string& name, uint32_t offset = 0, bool normalized = false);

		uint32_t GetComponentCount() const;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Vertex Buffer Layout ///////////////////////////////////////////////////////////////////////

	class GlVertexBufferLayout
	{
	public:
		GlVertexBufferLayout() {}

		GlVertexBufferLayout(const std::initializer_list<GlBufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		const uint32_t GetStride() const { return m_Stride; }
		const std::vector<GlBufferElement>& GetElements() const { return m_Elements; }

		std::vector<GlBufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<GlBufferElement>::iterator end() { return m_Elements.end(); }

		std::vector<GlBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<GlBufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetAndStride();

	private:
		std::vector<GlBufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Uniform Buffer Layout - std140 /////////////////////////////////////////////////////////////

	class GlUniformBufferLayout_std140
	{
	public:
		GlUniformBufferLayout_std140() = default;
		GlUniformBufferLayout_std140(uint32_t size, const std::initializer_list<GlBufferElement>& elements);

		const GlBufferElement& GetElement(const std::string& name) { return m_Elements[name]; }
		uint32_t Size() const { return m_Size; }

	private:
		std::unordered_map<std::string, GlBufferElement> m_Elements;
		uint32_t m_Size = 0;
	};

}