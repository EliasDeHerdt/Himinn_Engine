#pragma once
#include "Component.h"
#include <SDL.h>

using namespace std;
namespace Himinn {

	class Font;
	class TextComponent;
	class FPSComponent :
		public Component
	{
	public:
		FPSComponent(const std::weak_ptr<GameObject>& owner, const shared_ptr<Font>& font, const SDL_Color& color);
		virtual ~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
		int m_AccumulatedFrames = 0;
		double m_TimePassed = 0.0;

		// Components
		std::weak_ptr<TextComponent> m_TextComponent;
	};
}