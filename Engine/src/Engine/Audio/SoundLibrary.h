#pragma once
#include "Engine/Core/Base.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <irrKlang.h>

namespace Engine {

	class SoundLibrary
	{
	public:
		static void Add(const std::string& name, irrklang::ISoundSource* sound);;
		static irrklang::ISoundSource* Load(const std::string& name, const std::string& filepath, float volume = 1.0f);

		static irrklang::ISoundSource* Get(const std::string& name);
		static bool Exists(const std::string& name);

	private:
		static std::unordered_map<std::string, irrklang::ISoundSource*> s_Sounds;
	};

}