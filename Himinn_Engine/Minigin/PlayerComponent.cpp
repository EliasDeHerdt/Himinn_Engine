#include "MiniginPCH.h"
#include "PlayerComponent.h"

#include "SubjectComponent.h"
#include "GameObject.h"
#include "Observer.h"

Himinn::PlayerComponent::PlayerComponent(const std::weak_ptr<GameObject>& owner, int lives)
	: Component(owner)
	, m_Lives(lives)
	, m_Score(0)
{
	if (!owner.lock()->AddComponent<SubjectComponent>(make_shared<SubjectComponent>(owner)))
		std::cout << "PlayerComponent: A SubjectComponent was already present, so no new one was added.\n";
	m_pSubjectComponent = owner.lock()->GetComponent<SubjectComponent>();
}

int Himinn::PlayerComponent::GetLives() const
{
	return m_Lives;
}

int Himinn::PlayerComponent::GetScore() const
{
	return m_Score;
}

// Lose 1 life
void Himinn::PlayerComponent::LoseLife()
{
	if (m_Lives - 1 >= 0)
	{
		--m_Lives;
		m_pSubjectComponent.lock()->Notify(EventInfo{m_Lives, 0.f, ""}, ObserverEvent::PlayerDied);
	}
}

// Set lives to the given number.
// If this number is lower than 0, it is set to 0.
void Himinn::PlayerComponent::SetLives(int lives)
{
	if (lives >= 0)
	{
		m_Lives = lives;
		m_pSubjectComponent.lock()->Notify(EventInfo{ m_Lives, 0.f, "" }, ObserverEvent::PlayerDied);
	}
	else if (m_Lives != 0)
	{
		m_Lives = 0;
		m_pSubjectComponent.lock()->Notify(EventInfo{ m_Lives, 0.f, "" }, ObserverEvent::PlayerDied);
	}
}

void Himinn::PlayerComponent::GainScore(ScoreGain scoreGain)
{
	int score = (int)scoreGain;
	m_Score += score;
	m_pSubjectComponent.lock()->Notify(EventInfo{ m_Score, 0.f, "" }, ObserverEvent::PlayerScore);
}

void Himinn::PlayerComponent::FixedUpdate()
{
}

void Himinn::PlayerComponent::Update()
{
}

void Himinn::PlayerComponent::LateUpdate()
{
}

void Himinn::PlayerComponent::Render()
{
}
