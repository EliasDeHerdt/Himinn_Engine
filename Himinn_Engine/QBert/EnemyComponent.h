#pragma once
#include <memory>

#include "Component.h"
#include "DataTypes.h"

class GridComponent;

namespace Himinn {
	class ImageComponent;
	class GameObject;
}

class EnemyComponent : public virtual Himinn::Component
{
public:
	EnemyComponent(const std::weak_ptr<GridComponent>& grid, float moveDelay, int scoreOnKill);
	virtual ~EnemyComponent() override = default;
	EnemyComponent(const EnemyComponent& other) = delete;
	EnemyComponent& operator=(const EnemyComponent& other) = delete;
	EnemyComponent(EnemyComponent&& other) = delete;
	EnemyComponent& operator=(EnemyComponent&& other) = delete;

	virtual void FixedUpdate() override = 0;
	virtual void Update() override = 0;
	virtual void LateUpdate() override = 0;
	virtual void Render() override = 0;
	virtual void OnAddedToObject() override = 0;

	virtual void OnOverlap(std::weak_ptr<Himinn::GameObject> other) = 0;
protected:
	int m_ScoreOnKill;
	bool m_Active;
	float m_MoveTime;
	float m_MoveDelay;
	Himinn::IVector2 m_GridPosition;

	std::weak_ptr<GridComponent> m_pGridComponent;
	std::weak_ptr<Himinn::ImageComponent> m_pImageComponent;
	
	virtual void AddToNode() const;
	virtual void RemoveFromNode() const;
	virtual bool CheckValidMove() const;
};

