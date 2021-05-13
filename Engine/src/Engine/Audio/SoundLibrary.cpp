#include "egpch.h"
#include "SoundLibrary.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace Engine {

	std::map<std::string, Ref<Audio::SoundSource>> SoundLibrary::s_Sounds;
	Ref<Audio::SoundSource> SoundLibrary::s_Default;

	void SoundLibrary::Init()
	{
		s_Default = CreateRef<Audio::SoundSource>("DefaultSound", "assets/sounds/lightning-shoot.wav", 0.05f);
		Load();
	}

	void SoundLibrary::Load(const std::string& filepath)
	{
		Deserialize(filepath);
	}

	void SoundLibrary::Save(const std::string& filepath)
	{
		Serialize(filepath);
	}

	Ref<Audio::SoundSource> SoundLibrary::Create(const std::string& name, const std::string& filepath, float volume)
	{
		auto source = CreateRef<Audio::SoundSource>(name, filepath, volume);
		Add(source);
		return source;
	}

	Ref<Audio::SoundSource> SoundLibrary::Get(const std::string& name)
	{
		if (!Contains(name))
			return s_Default;
		return s_Sounds[name];
	}

	std::vector<std::string> SoundLibrary::GetNames()
	{
		std::vector<std::string> names;
		for (auto entry : s_Sounds)
			names.push_back(entry.first);
		return names;
	}

	bool SoundLibrary::Contains(const std::string& name)
	{
		return s_Sounds.find(name) != s_Sounds.end();
	}

	void SoundLibrary::Remove(const std::string& name)
	{
		s_Sounds.erase(name);
	}

	void SoundLibrary::Rename(const std::string oldName, const std::string newName)
	{
		ASSERT(!Contains(newName), "Sound already exists!");
		s_Sounds[newName] = s_Sounds[oldName];
		s_Sounds[newName]->m_Name = newName;
		Remove(oldName);
	}

	void SoundLibrary::Add(const Ref<Audio::SoundSource>& soundSource)
	{
		if (soundSource == nullptr)
			return;

		s_Sounds[soundSource->GetName()] = soundSource;
	}


	// Serialization
	static void SerializeSoundSource(YAML::Emitter& out, Ref<Audio::SoundSource> soundSource);
	static Ref<Audio::SoundSource> DeserializeSoundSource(const YAML::Node& soundSourceNode);


	void SoundLibrary::Serialize(const std::string& filepath)
	{
		LOG_INFO("Serialize SoundLibrary...\n\tfile: {}", filepath);

		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "SoundLibrary" << YAML::Value << "Untitled";

		// Sounds
		out << YAML::Key << "SoundSources" << YAML::Value << YAML::BeginSeq;
		for (auto& entry : s_Sounds)
			SerializeSoundSource(out, entry.second);
		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SoundLibrary::Deserialize(const std::string& filepath)
	{
		LOG_INFO("Deserialize SoundLibrary...\n\tfile: {}", filepath);

		YAML::Node data = YAML::LoadFile(filepath);

		ASSERT(data["SoundLibrary"], "No SoundLibrary node!");

		if (data["SoundSources"])
			for (YAML::Node soundSourceNode : data["SoundSources"])
			{
				Ref<Audio::SoundSource> soundSource = DeserializeSoundSource(soundSourceNode);
				Add(soundSource);
			}
	}

	void SerializeSoundSource(YAML::Emitter& out, Ref<Audio::SoundSource> soundSource)
	{
		out << YAML::BeginMap; // SoundSource
		out << YAML::Key << "SoundSource";
		out << YAML::Value << soundSource->GetName();

		out << YAML::Key << "Path";
		out << YAML::Value << soundSource->GetPath();

		out << YAML::Key << "Volume";
		out << YAML::Value << soundSource->GetVolume();

		out << YAML::EndMap; // SoundSource
	}

	Ref<Audio::SoundSource> DeserializeSoundSource(const YAML::Node& soundSourceNode)
	{
		if (!soundSourceNode["SoundSource"] || !soundSourceNode["Path"])
			return nullptr;

		std::string name = soundSourceNode["SoundSource"].as<std::string>();
		std::string path = soundSourceNode["Path"].as<std::string>();

		Ref<Audio::SoundSource> soundSource;

		if(soundSourceNode["Volume"])
			soundSource = CreateRef<Audio::SoundSource>(name, path, soundSourceNode["Volume"].as<float>());
		else
			soundSource = CreateRef<Audio::SoundSource>(name, path);

		return soundSource;
	}

}


