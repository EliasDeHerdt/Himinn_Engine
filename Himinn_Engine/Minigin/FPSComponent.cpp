#include "MiniginPCH.h"
#include "FPSComponent.h"


#include "GameObject.h"
#include "Time.h"

Himinn::FPSComponent::FPSComponent(const std::weak_ptr<GameObject>& owner, const shared_ptr<Font>& font, const SDL_Color& color)
	: Component(owner)
{
	if (!owner.lock()->AddComponent<TextComponent>(make_shared<TextComponent>(owner, "", font, color)))
		std::cout << "FPSComponent: A TextComponent was already present, so no new one was added\n";
	m_TextComponent = owner.lock()->GetComponent<TextComponent>();
}

void Himinn::FPSComponent::FixedUpdate()
{
}

void Himinn::FPSComponent::Update()
{
	++m_AccumulatedFrames;
	m_TimePassed += Time::GetInstance().GetDeltaTime();
	if (m_TimePassed >= 1.0) {

		//Show FPS on screen
		m_TextComponent.lock()->SetText(to_string(m_AccumulatedFrames) + " FPS");

		//Reset vars for next calculation
		m_TimePassed -= 1.0;
		m_AccumulatedFrames = 0;
	}
}

void Himinn::FPSComponent::LateUpdate()
{
}

void Himinn::FPSComponent::Render()
{
}