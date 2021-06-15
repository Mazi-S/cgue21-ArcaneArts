#pragma once
#include "Engine.h"
#include "Events/GameEnd.h"
#include "Events/MenuEvent.h"

class ExampleLayer : public Engine::Layer {
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGui() override;

	virtual void OnEvent(Engine::Event& event) override;

	bool OnGameEnd(GameEndEvent& event);
	bool OnMenu(MenuEvent& event);

private:
	Engine::Ref<Engine::Scene> m_Scene;

	// ImGui
	Engine::Scope<Engine::MaterialPanel> m_MaterialPanel;
	Engine::Scope<Engine::RendererPanel> m_RendererPanel;
	Engine::Scope<Engine::ShaderPanel> m_ShaderPanel;
	Engine::Scope<Engine::Texture2DPanel> m_TexturePanel;
	Engine::Scope<Engine::MeshPanel> m_MeshPanel;
	Engine::Scope<Engine::SceneHierarchyPanel> m_SceneHierarchyPanel;

	void SaveScene();
	void OpenScene();
	void NewScene();
};