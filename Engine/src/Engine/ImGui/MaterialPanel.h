#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Material.h"

namespace Engine {

	class MaterialPanel {
	public:
		MaterialPanel() = default;
		~MaterialPanel() = default;

		void OnImGui();

	private:
		bool m_Active = true;

		Ref<Material> m_SelectionContext;
	private:
		void DrawMaterialNode(Ref<Material>& material);
		void DrawMaterial(Ref<Material>& material);

		void Save();
	};

}