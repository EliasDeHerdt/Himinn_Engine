#include "SlickAndSamComponent.h"

#include <iostream>


#include "ControllerComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "GridComponent.h"
#include "ImageComponent.h"
#include "NodeComponent.h"

SlickAndSamComponent::SlickAndSamComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, float moveDelay)
	: Component(owner)
	, EnemyComponent(grid, moveDelay, 300)
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
		auto newImageComponent = std::make_shared<Himinn::ImageComponent>(m_Owner, "QBert/Characters/Character_Slick.png");
		m_Owner.lock()->AddComponent<Himinn::ImageComponent>(newImageComponent);
		m_pImageComponent = newImageComponent;
	}
}

void SlickAndSamComponent::OnOverlap(std::weak_ptr<Himinn::GameObject> other)
{
	if (m_Owner.expired())
		return;

	m_Owner.lock()->MarkForDestruction();
	auto controllerComp = other.lock()->GetComponent<ControllerComponent>();
	if (controllerComp.expired())
		return;

	controllerComp.lock()->GainScore(m_ScoreOnKill);
}

void SlickAndSamComponent::Spawn()
{
	if (m_pGridComponent.expired()
		|| m_pImageComponent.expired()
		|| m_pGridComponent.lock()->GetLayerAmount() < 2)
		return;
	
	m_Active = true;
	m_GridPosition = { 1, rand() % 2 };
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