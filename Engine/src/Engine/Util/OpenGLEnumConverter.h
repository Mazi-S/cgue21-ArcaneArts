#pragma once

#include <string>

namespace Engine::Util {

	class GlEnumConverter
	{
	public:
		static std::string ToString(uint32_t value);
	};

}