#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine {

	class RendererPanel {
	public:
		RendererPanel() = default;
		~RendererPanel() = default;

		void OnImGui();

	private:
		bool m_Active = true;

	};

}