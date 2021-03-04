#include "MiniginPCH.h"
#include "LivesComponent.h"

#include "TextComponent.h"
#include "GameObject.h"

Himinn::LivesComponent::LivesComponent(const std::weak_ptr<GameObject>& owner, int InitialLives, const shared_ptr<Font>& font, const SDL_Color& color)
	: Component(owner)
{
	if (!owner.lock()->AddComponent<TextComponent>(make_shared<TextComponent>(owner, "", font, color)))
		std::cout << "LivesComponent: A TextComponent was already present, so no new one was added\n";
	m_pTextComponent = owner.lock()->GetComponent<TextComponent>();
	m_pTextComponent.lock()->SetText("Amount of lives: " + std::to_string(InitialLives));
}

void Himinn::LivesComponent::FixedUpdate()
{
}

void Himinn::LivesComponent::Update()
{
}

void Himinn::LivesComponent::LateUpdate()
{
}

void Himinn::LivesComponent::Render()
{
}

// This function doesn't set an actuall value, this just sets the number in the string
void Himinn::LivesComponent::SetCurrentLives(int currentLives) const
{
	m_pTextComponent.lock()->SetText("Amount of lives: " + std::to_string(currentLives));
}