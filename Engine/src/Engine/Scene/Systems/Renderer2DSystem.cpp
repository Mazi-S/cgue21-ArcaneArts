#include "egpch.h"
#include "Renderer2DSystem.h"
#include "Engine/Scene/Components.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Util.h"

using SpriteRendererComponent			= Engine::Component::Renderer2D::SpriteRendererComponent;
using SpriteRendererTextureComponent	= Engine::Component::Renderer2D::SpriteRendererTextureComponent;
using TransformComponent				= Engine::Component::Core::TransformComponent;

namespace Engine::System::Renderer2D {

	void SubmitSprites(entt::registry& registry)
	{
		{
			auto view = registry.view<SpriteRendererComponent, TransformComponent>(entt::exclude<SpriteRendererTextureComponent>);
			for (auto entity : view)
			{
				auto& spriteRendererComp = view.get<SpriteRendererComponent>(entity);
				Engine::Renderer2D::DrawQuad(Util::Transform(registry, entity), spriteRendererComp.Color);
			}
		}

		{
			auto view = registry.view<SpriteRendererComponent, SpriteRendererTextureComponent, TransformComponent>();
			for (auto entity : view)
			{
				auto [spriteRendererComp, spriteRendererTextureComp] = view.get<SpriteRendererComponent, SpriteRendererTextureComponent>(entity);
				Engine::Renderer2D::DrawQuad(Util::Transform(registry, entity), spriteRendererTextureComp.Texture, spriteRendererComp.Color);
			}
		}
	}

}