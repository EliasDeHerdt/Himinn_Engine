#pragma once
#include "Component.h"
#include "TextComponent.h"

using namespace std;
namespace Himinn {

	class Font;
	class FPSComponent :
		public Component
	{
	public:
		FPSComponent(const std::weak_ptr<GameObject>& owner, const shared_ptr<Font>& font, const SDL_Color& color);

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