#include "QBertComponent.h"

#include <iostream>


#include "Observer.h"
#include "GameObject.h"
#include "PlayerObserver.h"
#include "ImageComponent.h"
#include "SubjectComponent.h"

QBertComponent::QBertComponent(const std::weak_ptr<Himinn::GameObject>& owner, int lives, std::string TexturePath)
	: Component(owner)
	, ControllableComponent()
	, m_Lives(lives)
	, m_Score()
	, m_TexturePath(TexturePath)
	, m_pImageComponent()
	, m_pSubjectComponent()
{
}

void QBertComponent::FixedUpdate()
{
}

void QBertComponent::Update()
{
}

void QBertComponent::LateUpdate()
{
}

void QBertComponent::Render()
{
}

void QBertComponent::OnAddedToObject()
{
	if (m_Owner.expired())
		return;
	
	m_pSubjectComponent = m_Owner.lock()->GetComponent<Himinn::SubjectComponent>();
	if (m_pSubjectComponent.expired())
		std::cout << "QBertComponent: No SubjectComponent was present, no observations will be made.\n";

	auto imageComponent = m_Owner.lock()->GetComponent<Himinn::ImageComponent>();
	if (!imageComponent.expired())
	{
		m_pImageComponent = imageComponent;
		m_pImageComponent.lock()->SetTexture(m_TexturePath);
	}
	else
	{
		std::cout << "QBertComponent: No ImageComponent found, Creating one.\n";

		auto newImageComponent = std::make_shared<Himinn::ImageComponent>(m_Owner, m_TexturePath);
		m_Owner.lock()->AddComponent<Himinn::ImageComponent>(newImageComponent);
		m_pImageComponent = newImageComponent;
	}
}

void QBertComponent::OnDeath()
{
	LoseLife();
}

void QBertComponent::OnScore(int amount)
{
	GainScore(amount);
}

int QBertComponent::GetLives() const
{
	return m_Lives;
}

int QBertComponent::GetScore() const
{
	return m_Score;
}

// Lose 1 life
void QBertComponent::LoseLife()
{
	if (m_Lives - 1 >= 0)
	{
		--m_Lives;
		if (!m_pSubjectComponent.expired())
			m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ {m_Lives}, {0.f}, {""} }, (unsigned)PlayerObserverEvent::PlayerDied);
	}
}

// Set lives to the given number.
// If this number is lower than 0, it is set to 0.
void QBertComponent::SetLives(int lives)
{
	if (lives >= 0)
	{
		m_Lives = lives;
		if (!m_pSubjectComponent.expired())
			m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ {m_Lives}, {0.f}, {""} }, (unsigned)PlayerObserverEvent::PlayerDied);
	}
	else if (m_Lives != 0)
	{
		m_Lives = 0;
		if (!m_pSubjectComponent.expired())
			m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ {m_Lives}, {0.f}, {""} }, (unsigned)PlayerObserverEvent::PlayerDied);
	}
}

void QBertComponent::SetScore(int score)
{
	m_Score = score;
	
	if (!m_pSubjectComponent.expired())
		m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ {m_Score}, {0.f}, {""} }, (unsigned)PlayerObserverEvent::PlayerScore);
}

void QBertComponent::GainScore(int scoreGain)
{
	m_Score += scoreGain;

	if (!m_pSubjectComponent.expired())
		m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ {m_Score}, {0.f}, {""} }, (unsigned)PlayerObserverEvent::PlayerScore);
}
