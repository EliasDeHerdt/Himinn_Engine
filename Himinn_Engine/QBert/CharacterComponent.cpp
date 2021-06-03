#include "CharacterComponent.h"

#include <iostream>
#include "Observer.h"
#include "GameObject.h"
#include "SubjectComponent.h"
#include "GridComponent.h"

CharacterComponent::CharacterComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, int lives)
	: Component(owner)
	, m_Lives(lives)
	, m_Score()
	, m_GridPosition()
	, m_pSubjectComponent()
	, m_pGridComponent(grid)
{
	if (!owner.lock()->AddComponent<Himinn::SubjectComponent>(make_shared<Himinn::SubjectComponent>(owner)))
		std::cout << "PlayerComponent: A SubjectComponent was already present, so no new one was added.\n";
	m_pSubjectComponent = owner.lock()->GetComponent<Himinn::SubjectComponent>();

	if (m_pGridComponent.expired())
	{
		std::cout << "PlayerComponent: The given grid does not exist.\n";
		return;
	}
	
	m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
}

int CharacterComponent::GetLives() const
{
	return m_Lives;
}

int CharacterComponent::GetScore() const
{
	return m_Score;
}

// Lose 1 life
void CharacterComponent::LoseLife()
{
	m_GridPosition = { 0, 0 };
	if (m_Lives - 1 >= 0)
	{
		--m_Lives;
		m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ m_Lives, 0.f, "" }, Himinn::ObserverEvent::PlayerDied);
	}
}

// Set lives to the given number.
// If this number is lower than 0, it is set to 0.
void CharacterComponent::SetLives(int lives)
{
	if (lives >= 0)
	{
		m_Lives = lives;
		m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ m_Lives, 0.f, "" }, Himinn::ObserverEvent::PlayerDied);
	}
	else if (m_Lives != 0)
	{
		m_Lives = 0;
		m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ m_Lives, 0.f, "" }, Himinn::ObserverEvent::PlayerDied);
	}
}

void CharacterComponent::GainScore(ScoreGain scoreGain)
{
	int score = (int)scoreGain;
	m_Score += score;
	m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ m_Score, 0.f, "" }, Himinn::ObserverEvent::PlayerScore);
}

void CharacterComponent::SetGridPosition(int layer, int number)
{
	SetGridPosition({ layer, number });
}

void CharacterComponent::SetGridPosition(Himinn::IVector2 position)
{
	m_GridPosition = position;
	Himinn::IVector2 nodeCharacterPosition = m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y);
	if (nodeCharacterPosition.x == -1
		|| nodeCharacterPosition.y == -1)
	{
		// LoseLife sets m_GridPosition to 0,0
		LoseLife();
		m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
	}
	else
		m_Owner.lock()->SetPosition(nodeCharacterPosition);
}

Himinn::IVector2 CharacterComponent::GetGridPosition() const
{
	return m_GridPosition;
}

void CharacterComponent::FixedUpdate()
{
}

void CharacterComponent::Update()
{
}

void CharacterComponent::LateUpdate()
{
}

void CharacterComponent::Render()
{
}
