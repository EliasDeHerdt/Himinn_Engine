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
		TextComponent(const std::weak_ptr<GameObject>& owner, const string& text, const shared_ptr<Font>& font, const SDL_Color& color);

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void SetText(const string& text);
		
	private:
		string m_Text;
		shared_ptr<Font> m_Font;
		SDL_Color m_Color;

		// Components
		std::weak_ptr<RenderComponent> m_RenderComponent;
	};
}