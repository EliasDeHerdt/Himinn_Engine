#include "MiniginPCH.h"
#include "ScoreComponent.h"

#include "TextComponent.h"
#include "GameObject.h"

Himinn::ScoreComponent::ScoreComponent(const std::weak_ptr<GameObject>& owner, int InitialScore, const shared_ptr<Font>& font, const SDL_Color& color)
	: Component(owner)
{
	if (!owner.lock()->AddComponent<TextComponent>(make_shared<TextComponent>(owner, "", font, color)))
		std::cout << "ScoreComponent: A TextComponent was already present, so no new one was added\n";
	m_pTextComponent = owner.lock()->GetComponent<TextComponent>();
	m_pTextComponent.lock()->SetText("Current score: " + std::to_string(InitialScore));
}

void Himinn::ScoreComponent::FixedUpdate()
{
}

void Himinn::ScoreComponent::Update()
{
}

void Himinn::ScoreComponent::LateUpdate()
{
}

void Himinn::ScoreComponent::Render()
{
}

// This function doesn't set an actuall value, this just sets the number in the string
void Himinn::ScoreComponent::SetCurrentScore(int currentScore) const
{
	m_pTextComponent.lock()->SetText("Current score: " + std::to_string(currentScore));
}