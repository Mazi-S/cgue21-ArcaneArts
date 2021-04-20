#pragma once

namespace Engine::Util {


	class Config
	{
	public:
		Config(const std::string& filepath);
		~Config() = default;

		float GetBrightness() { return m_Brightness; }
		float GetBrightness2D() { return m_Brightness2D; }

		float GetWidth() { return m_Width; }
		float GetHeight() { return m_Height; }
		float GetRefreshRate() { return m_RefreshRate; }
		float GetFullscreen() { return m_Fullscreen; }

	private:
		void Load(const std::string& filepath);

		std::string m_Filepath;

		float m_Brightness = 1.0;
		float m_Brightness2D = 1.0;

		int m_Width = 1280;
		int m_Height = 750;
		int m_RefreshRate = -1;
		bool m_Fullscreen = false;
	};

}