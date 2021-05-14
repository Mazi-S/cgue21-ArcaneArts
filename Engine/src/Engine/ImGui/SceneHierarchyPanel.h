#pragma once
#include "Engine/Core/Base.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"

namespace Engine {

	class SceneHierarchyPanel
	{
		using CallbackFn = std::function<void()>;

	public:
		SceneHierarchyPanel(CallbackFn newCallback, CallbackFn openCallback, CallbackFn saveCallback);
		~SceneHierarchyPanel() = default;

		void SetContext(const Ref<Scene>& context);

		void OnImGui();

	private:
		void DrawEntityNode(Entity entity);
		void DrawEntity(Entity entity);

		// Callback Functions
		CallbackFn m_NewCallback;
		CallbackFn m_OpenCallback;
		CallbackFn m_SaveCallback;

	private:
		bool m_Active = true;

		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}