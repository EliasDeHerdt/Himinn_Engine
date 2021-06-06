#include "CoilyComponent.h"


#include "ControllerComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "NodeComponent.h"
#include "GridComponent.h"
#include "ImageComponent.h"

CoilyComponent::CoilyComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, float moveDelay)
	: Component(owner)
	, EnemyComponent(grid, moveDelay, 500)
	, ControllableComponent()
	, m_Snake(false)
	, m_pControllerComponent()
{
}

void CoilyComponent::FixedUpdate()
{
}

void CoilyComponent::Update()
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

void CoilyComponent::LateUpdate()
{
}

void CoilyComponent::Render()
{
}

void CoilyComponent::OnAddedToObject()
{
	if (m_Owner.expired())
		return;

	auto imageComponent = m_Owner.lock()->GetComponent<Himinn::ImageComponent>();
	if (!imageComponent.expired())
	{
		m_pImageComponent = imageComponent;
		m_pImageComponent.lock()->SetTexture("QBert/Characters/Character_Coily_Egg.png");
	}
	else
	{
		auto newImageComponent = std::make_shared<Himinn::ImageComponent>(m_Owner, "QBert/Characters/Character_Coily_Egg.png");
		m_Owner.lock()->AddComponent<Himinn::ImageComponent>(newImageComponent);
		m_pImageComponent = newImageComponent;
	}
}

void CoilyComponent::OnDeath()
{
}

void CoilyComponent::OnScore(int)
{
}

void CoilyComponent::OnOverlap(std::weak_ptr<Himinn::GameObject> other)
{
	/*if (m_Owner.expired()
		|| other.expired())
		return;

	auto controllerComp = other.lock()->GetComponent<ControllerComponent>();
	if (controllerComp.expired())
		return;

	controllerComp.lock()->Die();*/
}

void CoilyComponent::Spawn()
{
	if (m_pGridComponent.expired()
		|| m_pImageComponent.expired())
		return;

	m_Active = true;

	m_Owner.lock()->SetPosition(m_pGridComponent.lock()->GetNodeCharacterPosition(m_GridPosition.x, m_GridPosition.y));
	AddToNode();
}

bool CoilyComponent::SetControllerComponent(std::weak_ptr<ControllerComponent> controllerComp)
{
	if (controllerComp.expired())
		return false;

	m_pControllerComponent = controllerComp;
	m_pControllerComponent.lock()->SetIsActive(false);
	return true;
}

void CoilyComponent::Move()
{
	if (m_pGridComponent.expired())
		return;

	int direction = rand() % 2;

	RemoveFromNode();
	if (!m_Snake)
	{
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

		CheckForHatch();
	}
	else if (m_pControllerComponent.expired())
	{
		
	}
}

void CoilyComponent::CheckForHatch()
{
	if (m_pGridComponent.expired()
		|| m_pImageComponent.expired()
		|| m_GridPosition.x != (int)m_pGridComponent.lock()->GetLayerAmount() - 1)
		return;

	m_Snake = true;
	m_pImageComponent.lock()->SetTexture("QBert/Characters/Character_Coily.png");

	if (m_pControllerComponent.expired())
		return;

	m_pControllerComponent.lock()->SetIsActive(true);
	m_pControllerComponent.lock()->SetGridPosition(m_GridPosition);
}