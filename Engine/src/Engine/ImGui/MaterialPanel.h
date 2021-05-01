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
		Ref<Material> m_SelectionContext;

	private:
		void DrawMaterialNode(Ref<Material>& material);
		void DrawMaterial(Ref<Material>& material);
	};

}