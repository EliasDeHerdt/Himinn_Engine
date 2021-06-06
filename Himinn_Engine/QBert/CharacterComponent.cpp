#include "CharacterComponent.h"

#include <iostream>
#include "Observer.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SubjectComponent.h"
#include "GridComponent.h"
#include "NodeComponent.h"
#include "QBertComponent.h"
#include "PlayerObserver.h"

CharacterComponent::CharacterComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid)
	: Component(owner)
	, m_GridPosition()
	, m_GridSpawnPosition()
	, m_CanMove(true)
	, m_MovementTimer()
	, m_MovementDelay(0.3f)
	, m_pGridComponent(grid)
{
	if (m_pGridComponent.expired())
		std::cout << "CharacterComponent: The given grid does not exist.\n";
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
	if (m_Owner.expired())
		return;

	auto qbertComp = m_Owner.lock()->GetComponent<QBertComponent>();
	if (qbertComp.expired())
		std::cout << "CharacterComponent: No QBerComponent found.";
	else
		m_pQbertComponent = qbertComp;
}

void CharacterComponent::Move(Himinn::QBertDirection direction)
{
	if (!m_CanMove
		|| m_pGridComponent.expired()
		|| m_pQbertComponent.expired()
		|| m_pQbertComponent.lock()->GetLives() == 0)
		return;

	m_CanMove = false;
	bool madeMove{ false };
	Himinn::IVector2 position{ GetGridPosition() };
	RemoveFromNode();
	
	switch (direction) {
		case Himinn::QBertDirection::TopLeft:
			madeMove = SetGridPosition(position.x - 1, position.y - 1);
			break;
		case Himinn::QBertDirection::TopRight:
			madeMove = SetGridPosition(position.x - 1, position.y);
			break;
		case Himinn::QBertDirection::BottomLeft:
			madeMove = SetGridPosition(position.x + 1, position.y);
			break;
		case Himinn::QBertDirection::BottomRight:
			madeMove = SetGridPosition(position.x + 1, position.y + 1);
		default: break;
	}

	if (!madeMove)
	{
		// LoseLife sets m_GridPosition to {0, 0}, but it still needs to be set.
		// This is because, if the life check succeeds, the player goes up WITHOUT losing a life.
		if (!m_pGridComponent.lock()->CheckForLift(m_GridPosition.x, m_GridPosition.y))
			m_pQbertComponent.lock()->LoseLife();
		
		m_GridPosition = { 0, 0 };
		SetGridPosition(m_GridPosition.x, m_GridPosition.y);
		return;
	}
	
	m_pGridComponent.lock()->UpgradeNode(m_GridPosition.x, m_GridPosition.y);
}

void CharacterComponent::MoveToSpawn()
{
	SetGridPosition(m_GridSpawnPosition);
}



void CharacterComponent::SetGrid(const std::weak_ptr<GridComponent>& grid)
{
	m_pGridComponent = grid;
}

bool CharacterComponent::SetGridPosition(int layer, int number)
{
	return SetGridPosition({ layer, number });
}

bool CharacterComponent::SetGridPosition(Himinn::IVector2 position)
{
	if (m_pGridComponent.expired())
		return false;
	
	m_GridPosition = position;
	Himinn::IVector2 nodeCharacterPosition = m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y);

	if (nodeCharacterPosition.x == -1
		|| nodeCharacterPosition.y == -1)
		return false;

	m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
	AddToNode();
	return true;
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

void CharacterComponent::AddToNode()
{
	if (m_Owner.expired()
		|| m_pGridComponent.expired())
		return;

	auto nodeObject = m_pGridComponent.lock()->GetNode(m_GridPosition.x, m_GridPosition.y);
	if (nodeObject.expired())
		return;

	auto nodeComp = nodeObject.lock()->GetComponent<NodeComponent>();
	if (nodeComp.expired())
		return;

	nodeComp.lock()->AddGameObject(m_Owner);
}

void CharacterComponent::RemoveFromNode()
{
	if (m_Owner.expired()
		|| m_pGridComponent.expired())
		return;
	
	auto nodeObject = m_pGridComponent.lock()->GetNode(m_GridPosition.x, m_GridPosition.y);
	if (nodeObject.expired())
		return;
	
	auto nodeComp = nodeObject.lock()->GetComponent<NodeComponent>();
	if (nodeComp.expired())
		return;
	
	nodeComp.lock()->RemoveGameObject(m_Owner);
}
