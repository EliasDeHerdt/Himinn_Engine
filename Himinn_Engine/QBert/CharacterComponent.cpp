#include "CharacterComponent.h"

#include <iostream>
#include "Observer.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SubjectComponent.h"
#include "GridComponent.h"

CharacterComponent::CharacterComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, int lives)
	: Component(owner)
	, m_Lives(lives)
	, m_Score()
	, m_GridPosition()
	, m_CanMove(true)
	, m_MovementTimer()
	, m_MovementDelay(0.3f)
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

void CharacterComponent::FixedUpdate()
{
}

void CharacterComponent::Update()
{
	if (!m_CanMove)
	{
		float dt = Himinn::GameTime::GetInstance().GetDeltaTime();
		
		m_MovementTimer += dt;
		if (m_MovementTimer >= m_MovementDelay)
		{
			m_CanMove = true;
			m_MovementTimer = 0.f;
		}
	}
}

void CharacterComponent::LateUpdate()
{
}

void CharacterComponent::Render()
{
}

int CharacterComponent::GetLives() const
{
	return m_Lives;
}

int CharacterComponent::GetScore() const
{
	return m_Score;
}

void CharacterComponent::Move(Himinn::QBertDirection direction)
{
	if (!m_CanMove)
		return;

	m_CanMove = false;
	Himinn::IVector2 position{ GetGridPosition() };
	
	switch (direction) {
		case Himinn::QBertDirection::TopLeft:
			SetGridPosition(position.x - 1, position.y - 1);
			break;
		case Himinn::QBertDirection::TopRight:
			SetGridPosition(position.x - 1, position.y);
			break;
		case Himinn::QBertDirection::BottomLeft:
			SetGridPosition(position.x + 1, position.y);
			break;
		case Himinn::QBertDirection::BottomRight:
			SetGridPosition(position.x + 1, position.y + 1);
		default: break;
	}
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
	bool update{ true };
	m_GridPosition = position;
	Himinn::IVector2 nodeCharacterPosition = m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y);

	if (nodeCharacterPosition.x == -1
		|| nodeCharacterPosition.y == -1)
	{
		// LoseLife sets m_GridPosition to {0, 0}, but it still needs to be set.
		// This is because, if the life check succeeds, the player goes up WITHOUT losing a life.
		if (!m_pGridComponent.lock()->CheckForLift(m_GridPosition.x, m_GridPosition.y))
			LoseLife();
		
		update = false;
		m_GridPosition = { 0, 0 };
	}
	
	m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
	if (update)
		m_pGridComponent.lock()->UpgradeNode(m_GridPosition.x, m_GridPosition.y);
}

Himinn::IVector2 CharacterComponent::GetGridPosition() const
{
	return m_GridPosition;
}