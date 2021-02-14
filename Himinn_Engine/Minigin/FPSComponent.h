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
		FPSComponent(const shared_ptr<Font>& font, const SDL_Color& color);

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render(const Transform& transform) override;

	private:
		TextComponent m_TextComponent;
		int m_AccumulatedFrames = 0;
		double m_TimePassed = 0.0;
	};
}