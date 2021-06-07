#include "UggAndWrongwayComponent.h"

#include <iostream>


#include "ControllerComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "GridComponent.h"
#include "ImageComponent.h"
#include "NodeComponent.h"
#include "ControllerComponent.h"

UggAndWrongwayComponent::UggAndWrongwayComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, float moveDelay)
	: Component(owner)
	, EnemyComponent(grid, moveDelay, 0)
	, m_TypeToSpawn(SpawnType::Ugg)
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
		m_MoveTime = 0;
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
		auto newImageComponent = std::make_shared<Himinn::ImageComponent>(m_Owner, "QBert/Characters/Character_Ugg.png");
		m_Owner.lock()->AddComponent<Himinn::ImageComponent>(newImageComponent);
		m_pImageComponent = newImageComponent;
	}
}

void UggAndWrongwayComponent::OnOverlap(std::weak_ptr<Himinn::GameObject> other)
{
	if (m_Owner.expired()
		|| other.expired())
		return;

	auto controllerComp = other.lock()->GetComponent<ControllerComponent>();
	if (controllerComp.expired())
		return;

	controllerComp.lock()->Die();
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

bool UggAndWrongwayComponent::CheckValidMove() const
{
	if (m_pGridComponent.expired())
		return false;
	
	Himinn::IVector2 nodeCharacterPosition = m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y);

	bool destroy{ false };
	if (nodeCharacterPosition.x == -1
		|| nodeCharacterPosition.y == -1)
		destroy = true;

	switch (m_TypeToSpawn) {
	case SpawnType::Ugg:
		if (m_GridPosition.y == 0)
			destroy = true;
		break;
	case SpawnType::Wrongway:
		if (m_GridPosition.y == m_GridPosition.x)
			destroy = true;
		break;
	default: 
		destroy = true;
		break;
	}

	if (destroy)
	{
		m_Owner.lock()->MarkForDestruction();
		return false;
	}
	return true;
}
