#pragma once
#include "Engine.h"

class HudLayer : public Engine::Layer {
public:
	HudLayer();
	virtual ~HudLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;

	virtual void OnEvent(Engine::Event& event) override;

private:
};