#pragma once
#include <glm/glm.hpp>

namespace Engine::OpenGL {

	struct Texture2DSpecification
	{
		uint32_t Width = 0, Height = 0;
		uint32_t Internalformat;
		uint32_t Format;
		uint32_t Type = 0x1401;

		// Texture Wrapping
		uint32_t Wrap_S = 0x2901;
		uint32_t Wrap_T = 0x2901;
		glm::vec4 Border;

		// Texture Filtering
		uint32_t Min_Filter = 0x2600;
		uint32_t Mag_Filter = 0x2600;

		// Texture Comparison Mode
		uint32_t ComparisonMode = 0;

		bool Mipmaps = true;
	};

	class GlTexture2D
	{
		friend class GlFramebuffer;

	public:
		GlTexture2D(const std::string& name, Texture2DSpecification spec);
		GlTexture2D(const std::string& name, const std::string& filepath);
		GlTexture2D(const std::string& name, Texture2DSpecification spec, const std::string& filepath);

		virtual ~GlTexture2D();

		virtual void Bind(uint32_t slot = 0) const;
		virtual const std::string& GetName() const { return m_Name; }
		virtual const std::string& GetPath() const { return m_Path; }

		virtual void SetData(const void* data, uint32_t size);

	private:
		void Create(const Texture2DSpecification& spec);
		void Create(Texture2DSpecification& spec, const std::string& filepath);

	private:
		std::uint32_t m_TextureID;
		std::string m_Name;
		std::string m_Path;

		Texture2DSpecification m_Specification;
	};

}
