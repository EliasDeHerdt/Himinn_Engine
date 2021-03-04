#pragma once
#include "Component.h"
#include <SDL.h>

namespace Himinn
{
	class Font;
	class TextComponent;
	class LivesComponent : public Component
	{
	public:
		LivesComponent(const std::weak_ptr<GameObject>& owner, int InitialLives, const shared_ptr<Font>& font, const SDL_Color& color);
		virtual ~LivesComponent() = default;
		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent(LivesComponent&& other) = delete;
		LivesComponent& operator=(LivesComponent&& other) = delete;
		
		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void SetCurrentLives(int currentLives) const;
	private:
		// Components
		std::weak_ptr<TextComponent> m_pTextComponent;
	};
}