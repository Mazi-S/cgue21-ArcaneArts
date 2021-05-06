#pragma once
#include "Engine/Core/Base.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"

namespace Engine {

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel(const Ref<Scene>& context);
		~SceneHierarchyPanel() = default;

		void OnImGui();

	private:
		void DrawEntityNode(Entity entity);
		void DrawEntity(Entity entity);

		void Save();

	private:
		bool m_Active = true;

		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}