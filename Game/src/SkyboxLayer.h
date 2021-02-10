#pragma once
#include "Engine.h"


class SkyboxLayer : public Engine::Layer {
public:
	SkyboxLayer();
	virtual ~SkyboxLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;

	virtual void OnEvent(Engine::Event& event) override;

private:
	Engine::Ref<Engine::Skybox> m_Skybox;
};
