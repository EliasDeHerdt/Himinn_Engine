#include "CharacterComponent.h"

#include <iostream>
#include "Observer.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SubjectComponent.h"
#include "GridComponent.h"
#include "PlayerObserver.h"

CharacterComponent::CharacterComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, int lives)
	: Component(owner)
	, m_Lives(lives)
	, m_Score()
	, m_GridPosition()
	, m_GridSpawnPosition()
	, m_CanMove(true)
	, m_CanUpgradeNodes(true)
	, m_MovementTimer()
	, m_MovementDelay(0.3f)
	, m_pSubjectComponent()
	, m_pGridComponent(grid)
{
	if (m_pGridComponent.expired())
		std::cout << "PlayerComponent: The given grid does not exist.\n";
	else
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
			m_MovementTimer -= m_MovementDelay;
		}
	}
}

void CharacterComponent::LateUpdate()
{
}

void CharacterComponent::Render()
{
}

void CharacterComponent::OnAddedToObject()
{
	m_pSubjectComponent = m_Owner.lock()->GetComponent<Himinn::SubjectComponent>();
	if (m_pSubjectComponent.expired())
		std::cout << "PlayerComponent: No SubjectComponent was present, no observations will be made.\n";
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
	if (!m_CanMove
		|| m_pGridComponent.expired())
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

	if (m_CanUpgradeNodes)
		m_pGridComponent.lock()->UpgradeNode(m_GridPosition.x, m_GridPosition.y);

	m_CanUpgradeNodes = true;
}

void CharacterComponent::MoveToSpawn()
{
	SetGridPosition(m_GridSpawnPosition);
}

// Lose 1 life
void CharacterComponent::LoseLife()
{
	m_GridPosition = { 0, 0 };
	if (m_Lives - 1 >= 0)
	{
		--m_Lives;
		if (!m_pSubjectComponent.expired())
			m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ {m_Lives}, {0.f}, {""} }, (unsigned)Himinn::ObserverEvent::PlayerDied);
	}
}

// Set lives to the given number.
// If this number is lower than 0, it is set to 0.
void CharacterComponent::SetLives(int lives)
{
	if (lives >= 0)
	{
		m_Lives = lives;
		if (!m_pSubjectComponent.expired())
			m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ {m_Lives}, {0.f}, {""} }, (unsigned)Himinn::ObserverEvent::PlayerDied);
	}
	else if (m_Lives != 0)
	{
		m_Lives = 0;
		if (!m_pSubjectComponent.expired())
			m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ {m_Lives}, {0.f}, {""} }, (unsigned)Himinn::ObserverEvent::PlayerDied);
	}
}

void CharacterComponent::GainScore(ScoreGain scoreGain)
{
	int score = (int)scoreGain;
	m_Score += score;
	
	if (!m_pSubjectComponent.expired())
		m_pSubjectComponent.lock()->Notify(Himinn::EventInfo{ {m_Score}, {0.f}, {""} }, (unsigned)Himinn::ObserverEvent::PlayerScore);
}

void CharacterComponent::SetGrid(const std::weak_ptr<GridComponent>& grid)
{
	m_pGridComponent = grid;
}

void CharacterComponent::SetGridPosition(int layer, int number)
{
	SetGridPosition({ layer, number });
}

void CharacterComponent::SetGridPosition(Himinn::IVector2 position)
{
	if (m_pGridComponent.expired())
		return;
	
	m_GridPosition = position;
	Himinn::IVector2 nodeCharacterPosition = m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y);

	if (nodeCharacterPosition.x == -1
		|| nodeCharacterPosition.y == -1)
	{
		// LoseLife sets m_GridPosition to {0, 0}, but it still needs to be set.
		// This is because, if the life check succeeds, the player goes up WITHOUT losing a life.
		if (!m_pGridComponent.lock()->CheckForLift(m_GridPosition.x, m_GridPosition.y))
			LoseLife();
		
		m_GridPosition = { 0, 0 };
		m_CanUpgradeNodes = false;
	}
	
	m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
}

void CharacterComponent::SetGridSpawnPosition(int layer, int number)
{
	SetGridSpawnPosition({ layer, number });
}

void CharacterComponent::SetGridSpawnPosition(Himinn::IVector2 position)
{
	m_GridSpawnPosition = position;
}

Himinn::IVector2 CharacterComponent::GetGridPosition() const
{
	return m_GridPosition;
}
