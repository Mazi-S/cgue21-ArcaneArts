#include "egpch.h"
#include "OpenGLBufferLayout.h"
#include "OpenGLUtil.h"

namespace Engine::OpenGL {

	GlBufferElement::GlBufferElement(GlShaderDataType type, const std::string& name, uint32_t offset, bool normalized)
		: Name(name), Type(type), Size(Util::ShaderDataTypeSize(type)), Offset(offset), Normalized(normalized)
	{ }

	uint32_t GlBufferElement::GetComponentCount() const
	{
		switch (Type)
		{
		case GlShaderDataType::None:		return 0;
		case GlShaderDataType::Struct:		return 0;
		case GlShaderDataType::Float:		return 1;
		case GlShaderDataType::Float2:		return 2;
		case GlShaderDataType::Float3:		return 3;
		case GlShaderDataType::Float4:		return 4;
		case GlShaderDataType::Mat3:		return 3 * 3;
		case GlShaderDataType::Mat4:		return 4 * 4;
		case GlShaderDataType::Int:			return 1;
		case GlShaderDataType::Int2:		return 2;
		case GlShaderDataType::Int3:		return 3;
		case GlShaderDataType::Int4:		return 4;
		case GlShaderDataType::Bool:		return 1;
		}

		ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Vertex Buffer Layout ///////////////////////////////////////////////////////////////////////

	void GlVertexBufferLayout::CalculateOffsetAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;

		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Uniform Buffer Layout - std140 /////////////////////////////////////////////////////////////

	GlUniformBufferLayout_std140::GlUniformBufferLayout_std140(uint32_t size, const std::initializer_list<GlBufferElement>& elements)
		: m_Size(size)
	{
		for (auto& element : elements)
			m_Elements[element.Name] = element;
	}

}