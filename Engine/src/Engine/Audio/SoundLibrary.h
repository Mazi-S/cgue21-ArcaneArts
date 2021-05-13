#pragma once
#include "Engine/Core/Base.h"

#include "SoundSource.h"

#include <glm/glm.hpp>
#include <map>

namespace Engine {

	class SoundLibrary
	{
	public:
		static void Init();

		static void Load(const std::string& filepath = "assets/SoundLibrary.yaml");
		static void Save(const std::string& filepath = "assets/SoundLibrary.yaml");

		static Ref<Audio::SoundSource> Create(const std::string& name, const std::string& filepath, float volume = 1.0f);

		static Ref<Audio::SoundSource> Get(const std::string& name);
		static std::vector<std::string> GetNames();

		static bool Contains(const std::string& name);

		static void Remove(const std::string& name);
		static void Rename(const std::string oldName, const std::string newName);

	private:
		static void Add(const Ref<Audio::SoundSource>& soundSource);

		static void Serialize(const std::string& filepath);
		static void Deserialize(const std::string& filepath);

	private:
		static std::map<std::string, Ref<Audio::SoundSource>> s_Sounds;
		static Ref<Audio::SoundSource> s_Default;
	};

}