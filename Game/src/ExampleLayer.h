#pragma once
#include "Engine.h"
#include "Events/GameEnd.h"

class ExampleLayer : public Engine::Layer {
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;
	virtual void OnImGui() override;

	virtual void OnEvent(Engine::Event& event) override;

	bool OnKeyPressed(Engine::KeyPressedEvent& event);
	bool OnGameEnd(GameEndEvent& event);

private:
	Engine::Ref<Engine::Scene> m_Scene;
	Engine::Ref<Engine::Skybox> m_Skybox;

	Engine::Entity m_Hero;

	bool m_Menu = false;

	Engine::Ref<Engine::ParticleSystem> m_ParticleSystem;

	// ImGui
	Engine::Scope<Engine::MaterialPanel> m_MaterialPanel;
	Engine::Scope<Engine::ShaderPanel> m_ShaderPanel;
	Engine::Scope<Engine::Texture2DPanel> m_TexturePanel;
	Engine::Scope<Engine::MeshPanel> m_MeshPanel;
	Engine::Scope<Engine::SceneHierarchyPanel> m_SceneHierarchyPanel;
};