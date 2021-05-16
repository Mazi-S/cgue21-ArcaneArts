#pragma once
#include "Engine/Core/Base.h"
#include "SceneHierarchy.h"

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
		void DrawEntity(Entity entity);

		void DrawAddComponent(Entity entity);
		void DrawComponents(Entity entity);

		// implemented on client side (game)
		void DrawAddGameComponent(Entity entity);
		void DrawGameComponents(Entity entity);

		// Callback Functions
		CallbackFn m_NewCallback;
		CallbackFn m_OpenCallback;
		CallbackFn m_SaveCallback;

	private:
		bool m_Active = true;

		SceneHierarchy m_Hierarchy;

		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}