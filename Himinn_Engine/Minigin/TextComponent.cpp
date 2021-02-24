#include "MiniginPCH.h"
#include "TextComponent.h"

#include <SDL_ttf.h>
#include "RenderComponent.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "Font.h"

Himinn::TextComponent::TextComponent(const std::weak_ptr<GameObject>& owner, const string& text, const shared_ptr<Font>& font, const SDL_Color& color)
	: Component(owner)
	, m_Text(text)
	, m_Font(font)
	, m_Color{color}
{
	if (!owner.lock()->AddComponent<RenderComponent>(make_shared<RenderComponent>(owner)))
		std::cout << "ImageComponent: A RenderComponent was already present, so no new one was added\n";
	m_RenderComponent = owner.lock()->GetComponent<RenderComponent>();
	if (!text.empty())
		m_NeedsUpdate = true;
}

void Himinn::TextComponent::FixedUpdate()
{
}

void Himinn::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		//Create text-based texture
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);

		//Push texture to RenderComponent
		m_RenderComponent.lock()->SetTexture(make_shared<Texture2D>(texture));

		//Disable the need to update
		m_NeedsUpdate = false;
	}
}

void Himinn::TextComponent::LateUpdate()
{
}

void Himinn::TextComponent::Render()
{
}

// This implementation uses the "dirty flag" pattern
void Himinn::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}