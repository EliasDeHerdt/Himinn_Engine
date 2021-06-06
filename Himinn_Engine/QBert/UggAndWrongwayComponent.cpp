#include "UggAndWrongwayComponent.h"

#include <iostream>

#include "GameObject.h"
#include "GameTime.h"
#include "GridComponent.h"
#include "ImageComponent.h"
#include "NodeComponent.h"

UggAndWrongwayComponent::UggAndWrongwayComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, float moveDelay)
	: EnemyComponent(owner)
	, m_Active()
	, m_MoveTime()
	, m_MoveDelay(moveDelay)
	, m_TypeToSpawn()
	, m_GridPosition()
	, m_pGridComponent(grid)
	, m_pImageComponent()
{
}

void UggAndWrongwayComponent::FixedUpdate()
{
}

void UggAndWrongwayComponent::Update()
{
	if (!m_Active
		|| m_pGridComponent.expired())
		return;

	float dt = Himinn::GameTime::GetInstance().GetDeltaTime();
	m_MoveTime += dt;
	if (m_MoveTime >= m_MoveDelay)
	{
		m_MoveTime -= m_MoveDelay;
		Move();
	}
}

void UggAndWrongwayComponent::LateUpdate()
{
}

void UggAndWrongwayComponent::Render()
{
}

void UggAndWrongwayComponent::OnAddedToObject()
{
	if (m_Owner.expired())
		return;

	auto imageComponent = m_Owner.lock()->GetComponent<Himinn::ImageComponent>();
	if (!imageComponent.expired())
	{
		m_pImageComponent = imageComponent;
		m_pImageComponent.lock()->SetTexture("QBert/Characters/Character_Ugg.png");
	}
	else
	{
		std::cout << "UggAndWrongwayComponent: No ImageComponent found, Creating one.\n";

		auto newImageComponent = std::make_shared<Himinn::ImageComponent>(m_Owner, "QBert/Characters/Character_Ugg.png");
		m_Owner.lock()->AddComponent<Himinn::ImageComponent>(newImageComponent);
		m_pImageComponent = newImageComponent;
	}
}

void UggAndWrongwayComponent::OnOverlap()
{
	if (m_Owner.expired())
		return;

	m_Owner.lock()->MarkForDestruction();
}

void UggAndWrongwayComponent::Spawn()
{
	if (m_pGridComponent.expired()
		|| m_pImageComponent.expired())
		return;

	m_Active = true;
	m_TypeToSpawn = SpawnType(rand() % 2);
	int layerAmount = (int)m_pGridComponent.lock()->GetLayerAmount();
	switch (m_TypeToSpawn) {
		case SpawnType::Ugg:
			m_GridPosition = { layerAmount - 1, layerAmount - 1 };
			break;
		case SpawnType::Wrongway:
			m_pImageComponent.lock()->SetTexture("QBert/Characters/Character_Wrongway.png");
			m_GridPosition = { layerAmount - 1, 0 };
			break;
		default: break;
	}

	m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
	AddToNode();
}

void UggAndWrongwayComponent::Move()
{
	if (m_pGridComponent.expired())
		return;

	int direction = rand() % 2;

	RemoveFromNode();
	switch (direction)
	{
	case 0:
	{
		m_GridPosition.y += m_TypeToSpawn == SpawnType::Ugg ? -1 : 1;
		if (!CheckValidMove())
			break;

		m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
		AddToNode();
		break;
	}
	case 1:
	{
		--m_GridPosition.x;
		if (m_TypeToSpawn == SpawnType::Ugg)
			--m_GridPosition.y;
			
		if (!CheckValidMove())
			break;

		m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
		AddToNode();
		break;
	}
	default: break;
	}
}

void UggAndWrongwayComponent::AddToNode() const
{
	if (m_Owner.expired())
		return;

	auto nodeObject = m_pGridComponent.lock()->GetNode(m_GridPosition.x, m_GridPosition.y);
	if (nodeObject.expired())
		return;

	auto nodeComp = nodeObject.lock()->GetComponent<NodeComponent>();
	if (nodeComp.expired())
		return;
	
	nodeComp.lock()->AddGameObject(m_Owner);
}

void UggAndWrongwayComponent::RemoveFromNode() const
{
	if (m_Owner.expired())
		return;

	auto nodeObject = m_pGridComponent.lock()->GetNode(m_GridPosition.x, m_GridPosition.y);
	if (nodeObject.expired())
		return;

	auto nodeComp = nodeObject.lock()->GetComponent<NodeComponent>();
	if (nodeComp.expired())
		return;

	nodeComp.lock()->RemoveGameObject(m_Owner);
}

bool UggAndWrongwayComponent::CheckValidMove() const
{
	Himinn::IVector2 nodeCharacterPosition = m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y);

	if (nodeCharacterPosition.x == -1
		|| nodeCharacterPosition.y == -1)
	{
		m_Owner.lock()->MarkForDestruction();
		return false;
	}
	return true;
}