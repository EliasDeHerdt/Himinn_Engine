#include "EnemyComponent.h"

#include "GameObject.h"
#include "NodeComponent.h"
#include "GridComponent.h"

EnemyComponent::EnemyComponent(const std::weak_ptr<GridComponent>& grid, float moveDelay, int scoreOnKill)
	: m_ScoreOnKill(scoreOnKill)
	, m_Active()
	, m_MoveTime()
	, m_MoveDelay(moveDelay)
	, m_GridPosition()
	, m_pGridComponent(grid)
	, m_pImageComponent()
{
	
}

void EnemyComponent::AddToNode() const
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

void EnemyComponent::RemoveFromNode() const
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

bool EnemyComponent::CheckValidMove() const
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