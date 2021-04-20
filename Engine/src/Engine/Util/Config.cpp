#include "egpch.h"
#include "Config.h"
#include <yaml-cpp/yaml.h>

namespace Engine::Util {

	Config::Config(const std::string& filepath)
		: m_Filepath(filepath)
	{
		Load(filepath);
	}

	void Config::Load(const std::string& filepath)
	{
		YAML::Node config = YAML::LoadFile(filepath);

		if (config["application"])
		{
			YAML::Node application = config["application"];

			if(application["brightness"])
				m_Brightness = application["brightness"].as<float>();

			if (application["brightness2D"])
				m_Brightness2D = application["brightness2D"].as<float>();
		}

		if (config["window"])
		{
			YAML::Node window = config["window"];

			if(window["width"])
				m_Width = window["width"].as<int>();
			if(window["height"])
				m_Height = window["height"].as<int>();
			if(window["refreshRate"])
				m_RefreshRate = window["refreshRate"].as<int>();
			if(window["fullscreen"])
				m_Fullscreen = window["fullscreen"].as<bool>();
		}

	}

}

