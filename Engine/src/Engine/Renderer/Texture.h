#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Engine/Core/Base.h"

namespace Engine {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;

		static Ref<Texture> Create(const std::string& filepath);
	};

}