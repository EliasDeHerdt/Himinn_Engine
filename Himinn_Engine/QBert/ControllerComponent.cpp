#include "ControllerComponent.h"

#include <iostream>
#include "Observer.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SubjectComponent.h"
#include "GridComponent.h"
#include "NodeComponent.h"
#include "QBertComponent.h"
#include "PlayerObserver.h"

ControllerComponent::ControllerComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid)
	: Component(owner)
	, m_GridPosition()
	, m_GridSpawnPosition()
	, m_EnableMovement(true)
	, m_OnLift(false)
	, m_CanMove(true)
	, m_IsQbert(false)
	, m_MovementTimer()
	, m_MovementDelay(0.3f)
	, m_LiftTimer()
	, m_LiftDelay(2.f)
	, m_pGridComponent(grid)
{
	if (m_pGridComponent.expired())
		std::cout << "CharacterComponent: The given grid does not exist.\n";
	else
		m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
}

void ControllerComponent::FixedUpdate()
{
}

void ControllerComponent::Update()
{
	float dt = Himinn::GameTime::GetInstance().GetDeltaTime();
	
	if (!m_EnableMovement)
		return;
	
	if (!m_CanMove)
	{
		m_MovementTimer += dt;
		if (m_MovementTimer >= m_MovementDelay)
		{
			m_CanMove = true;
			m_MovementTimer = 0;
		}
	}
	
	if (m_OnLift)
	{
		m_LiftTimer += dt;
		if (m_LiftTimer >= m_LiftDelay)
		{
			m_OnLift = false;
			m_LiftTimer = 0;
			m_GridPosition = { 0, 0 };
			m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
		}
	}
}

void ControllerComponent::LateUpdate()
{
}

void ControllerComponent::Render()
{
}

void ControllerComponent::OnAddedToObject()
{
	if (m_Owner.expired())
		return;

	auto controllableComp = m_Owner.lock()->GetComponent<ControllableComponent>();
	if (controllableComp.expired())
		std::cout << "ControllerComponent: No ControllableComponent found.";
	else
	{
		m_pControllableComponent = controllableComp;
		m_IsQbert = !m_Owner.lock()->GetComponent<QBertComponent>().expired();
	}
}

void ControllerComponent::Move(Himinn::QBertDirection direction)
{
	if (!m_EnableMovement
		|| !m_CanMove
		|| m_OnLift
		|| m_pGridComponent.expired())
		return;

	m_CanMove = false;
	bool madeMove{ false };
	Himinn::IVector2 position{ GetGridPosition() };
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
		// If the life check succeeds, the player goes up WITHOUT losing a life.
		if (!m_IsQbert
			|| !m_pGridComponent.lock()->CheckForLift(m_GridPosition.x, m_GridPosition.y))
		{
			Die();
		}
		else
			m_OnLift = true;
		
		return;
	}

	if (m_IsQbert)
		m_pGridComponent.lock()->UpgradeNode(m_GridPosition.x, m_GridPosition.y);
}

void ControllerComponent::MoveToSpawn()
{
	//Extra AddToNode for safety
	SetGridPosition(m_GridSpawnPosition);
	AddToNode();
}

void ControllerComponent::Die()
{
	if (m_pControllableComponent.expired())
		return;
	
	m_pControllableComponent.lock()->OnDeath();
	
	m_GridPosition = m_GridSpawnPosition;
	SetGridPosition(m_GridPosition.x, m_GridPosition.y);
}

void ControllerComponent::GainScore(int score)
{
	if (m_pControllableComponent.expired())
		return;
	
	m_pControllableComponent.lock()->OnScore(score);
}

void ControllerComponent::SetEnableMovement(bool state)
{
	m_EnableMovement = state;
}

void ControllerComponent::SetGrid(const std::weak_ptr<GridComponent>& grid)
{
	m_pGridComponent = grid;
}

bool ControllerComponent::SetGridPosition(int layer, int number)
{
	return SetGridPosition({ layer, number });
}

bool ControllerComponent::SetGridPosition(Himinn::IVector2 position)
{
	if (m_pGridComponent.expired())
		return false;

	bool samePos = (m_GridPosition == position);

	if (!samePos)
		RemoveFromNode();
	
	m_GridPosition = position;
	Himinn::IVector2 nodeCharacterPosition = m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y);

	if (nodeCharacterPosition.x == -1
		|| nodeCharacterPosition.y == -1)
		return false;

	m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));

	if (!samePos)
		AddToNode();

	return true;
}

void ControllerComponent::SetGridSpawnPosition(int layer, int number)
{
	SetGridSpawnPosition({ layer, number });
}

void ControllerComponent::SetGridSpawnPosition(Himinn::IVector2 position)
{
	m_GridSpawnPosition = position;
}

Himinn::IVector2 ControllerComponent::GetGridPosition() const
{
	return m_GridPosition;
}

void ControllerComponent::AddToNode()
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

void ControllerComponent::RemoveFromNode()
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
