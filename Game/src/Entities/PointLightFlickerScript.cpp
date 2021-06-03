#include "MonsterScript.h"
#include "Components/GameComponents.h"
#include "PointLightFlickerScript.h"
#include <random>

using PointLightComponent = Engine::Component::Renderer::PointLightComponent;

void PointLightFlickerScript::OnUpdate(Engine::Timestep ts)
{
	auto [flickerComp, pointLightComp] = m_RegistryHandle->try_get<PointLightFlickerComponent, PointLightComponent>(m_EntityHandle);

	if (flickerComp == nullptr || pointLightComp == nullptr)
		return;

	if (flickerComp->TimeLeft <= 0)
	{
		pointLightComp->Color = flickerComp->Color;

		static std::random_device rand_dev;
		static std::mt19937 generator(rand_dev());
		static std::normal_distribution<float> distr(-0.5f, 0.5f);;

		float deltaTime = flickerComp->Duration + flickerComp->DurationVariation * distr(generator);
		float targetIntensity = flickerComp->Intensity + flickerComp->IntensityVariation * distr(generator);

		flickerComp->TimeLeft = deltaTime;
		flickerComp->IntensityDelta = flickerComp->Color * ((targetIntensity - 1.0f) / deltaTime);
	}
	else
	{
		pointLightComp->Color += flickerComp->IntensityDelta * (float)ts;
		flickerComp->TimeLeft -= ts;
	}
}
