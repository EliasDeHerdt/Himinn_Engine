#include "MiniginPCH.h"
#include "FPSComponent.h"

#include "Time.h"

Himinn::FPSComponent::FPSComponent(const shared_ptr<Font>& font, const SDL_Color& color)
	: m_TextComponent{"0 FPS", font, color}
{
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
		m_TextComponent.SetText(to_string(m_AccumulatedFrames) + " FPS");

		//Reset vars for next calculation
		m_TimePassed -= 1.0;
		m_AccumulatedFrames = 0;
	}

	//Update our component
	m_TextComponent.Update();
}

void Himinn::FPSComponent::LateUpdate()
{
}

void Himinn::FPSComponent::Render(const Transform& transform)
{
	m_TextComponent.Render(transform);
}