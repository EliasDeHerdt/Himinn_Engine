#include "SlickAndSamComponent.h"

#include <iostream>

#include "GameObject.h"
#include "GameTime.h"
#include "GridComponent.h"
#include "ImageComponent.h"
#include "NodeComponent.h"

SlickAndSamComponent::SlickAndSamComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, float moveDelay)
	: EnemyComponent(owner)
	, m_Active()
	, m_MoveTime()
	, m_MoveDelay(moveDelay)
	, m_GridPosition({ 1, rand() % 2 })
	, m_pGridComponent(grid)
	, m_pImageComponent()
{
}

void SlickAndSamComponent::FixedUpdate()
{
}

void SlickAndSamComponent::Update()
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

void SlickAndSamComponent::LateUpdate()
{
}

void SlickAndSamComponent::Render()
{
}

void SlickAndSamComponent::OnAddedToObject()
{
	if (m_Owner.expired())
		return;

	auto imageComponent = m_Owner.lock()->GetComponent<Himinn::ImageComponent>();
	if (!imageComponent.expired())
	{
		m_pImageComponent = imageComponent;
		m_pImageComponent.lock()->SetTexture("QBert/Characters/Character_Slick.png");
	}
	else
	{
		std::cout << "SlickAndSamComponent: No ImageComponent found, Creating one.\n";
		
		auto newImageComponent = std::make_shared<Himinn::ImageComponent>(m_Owner, "QBert/Characters/Character_Slick.png");
		m_Owner.lock()->AddComponent<Himinn::ImageComponent>(newImageComponent);
		m_pImageComponent = newImageComponent;
	}
}

void SlickAndSamComponent::OnOverlap()
{
	if (m_Owner.expired())
		return;

	m_Owner.lock()->MarkForDestruction();
}

void SlickAndSamComponent::Spawn()
{
	if (m_pGridComponent.expired()
		|| m_pImageComponent.expired()
		|| m_pGridComponent.lock()->GetLayerAmount() < 2)
		return;
	
	m_Active = true;
	SpawnType typeToSpawn = SpawnType(rand() % 2);

	if (typeToSpawn == SpawnType::Sam)
		m_pImageComponent.lock()->SetTexture("QBert/Characters/Character_Sam.png");

	m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
	AddToNode();
}

void SlickAndSamComponent::Move()
{
	if (m_pGridComponent.expired())
		return;
	
	int direction = rand() % 2;

	RemoveFromNode();
	switch (direction)
	{
		case 0:
		{
			++m_GridPosition.x;
			if (!CheckValidMove())
				break;

			m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
			AddToNode();
			break;
		}
		case 1:
		{
			++m_GridPosition.x;
			++m_GridPosition.y;
			if (!CheckValidMove())
				break;

			m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
			AddToNode();
			break;
		}
		default: break;
	}
}

void SlickAndSamComponent::AddToNode() const
{
	if (m_Owner.expired())
		return;
	
	auto nodeObject = m_pGridComponent.lock()->GetNode(m_GridPosition.x, m_GridPosition.y);
	if (nodeObject.expired())
		return;

	auto nodeComp = nodeObject.lock()->GetComponent<NodeComponent>();
	if (nodeComp.expired())
		return;

	nodeComp.lock()->SetNodeLevel(0);
	nodeComp.lock()->AddGameObject(m_Owner);
}

void SlickAndSamComponent::RemoveFromNode() const
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

bool SlickAndSamComponent::CheckValidMove() const
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
