#pragma once
#include "Engine.h"

class LayerTemplate : public Engine::Layer {
public:
	LayerTemplate();
	virtual ~LayerTemplate() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::Timestep ts) override;

	virtual void OnEvent(Engine::Event& event) override;

private:
};