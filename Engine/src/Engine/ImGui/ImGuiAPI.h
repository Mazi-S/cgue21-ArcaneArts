#pragma once
#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"

namespace Engine {

	class ImGuiAPI {
	public:
		static void Init();
		static void Shutdown();

		static void OnEvent(Event & e);

		static void Begin();
		static void End();

		static void BlockEvents(bool block) { s_BlockEvents = block; }

	private:
		static void SetDarkThemeColors();
	
	private:
		static bool s_BlockEvents;
		static Timestep m_Time;
	};

}