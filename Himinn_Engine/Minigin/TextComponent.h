#pragma once
#include "Component.h"
#include "RenderComponent.h"
#include <SDL.h>

using namespace std;
namespace Himinn {

	class Font;
	class TextComponent : public Component
	{
	public:
		TextComponent(const string& text, const shared_ptr<Font>& font, const SDL_Color& color);

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render(const Transform& transform) override;

		void SetText(const string& text);
		
	private:
		string m_Text;
		shared_ptr<Font> m_Font;
		RenderComponent m_RenderComponent;
		SDL_Color m_Color;
	};
}