#pragma once
#include "Component.h"
#include <SDL.h>

namespace Himinn
{
	class Font;
	class TextComponent;
	class ScoreComponent : public Component
	{
	public:
		ScoreComponent(const std::weak_ptr<GameObject>& owner, int InitialScore, const std::shared_ptr<Font>& font, const SDL_Color& color);
		virtual ~ScoreComponent() = default;
		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void SetCurrentScore(int currentScore) const;
	private:
		// Components
		std::weak_ptr<TextComponent> m_pTextComponent;
	};
}