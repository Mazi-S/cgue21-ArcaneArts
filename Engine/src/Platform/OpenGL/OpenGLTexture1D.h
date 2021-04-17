#pragma once
#include <glm/glm.hpp>

namespace Engine::OpenGL {

	struct Texture1DSpecification
	{
		uint32_t Width = 0;
		uint32_t Internalformat;
		uint32_t Format;
		uint32_t Type;

		// Texture Wrapping
		uint32_t Wrap_S = 0x2901;

		// Texture Filtering
		uint32_t Min_Filter = 0x2600;
		uint32_t Mag_Filter = 0x2600;
	};

	class GlTexture1D
	{
	public:
		GlTexture1D(const std::string& name, Texture1DSpecification spec);

		virtual ~GlTexture1D();

		virtual void Bind(uint32_t slot = 0) const;

		void GlTexture1D::SetData(const void* data, uint32_t size);

	private:
		void Create(const Texture1DSpecification& spec);

	private:
		std::uint32_t m_TextureID;
		std::string m_Name;

		Texture1DSpecification m_Specification;
	};


}