#pragma once
#include <memory>

#include "Component.h"
#include "DataTypes.h"

namespace Himinn {
	class GameObject;
	class SubjectComponent;
}

class GridComponent;
class ControllableComponent;
class ControllerComponent : public Himinn::Component
{
public:
	ControllerComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid);
	virtual ~ControllerComponent() override = default;
	ControllerComponent(const ControllerComponent& other) = delete;
	ControllerComponent& operator=(const ControllerComponent& other) = delete;
	ControllerComponent(ControllerComponent&& other) = delete;
	ControllerComponent& operator=(ControllerComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

	void Move(Himinn::QBertDirection direction);
	void MoveToSpawn();
	void Die();
	void GainScore(int score);
	
	void SetEnableMovement(bool state);
	void SetGrid(const std::weak_ptr<GridComponent>& grid);
	bool SetGridPosition(int layer, int number);
	bool SetGridPosition(Himinn::IVector2 position);
	void SetGridSpawnPosition(int layer, int number);
	void SetGridSpawnPosition(Himinn::IVector2 position);
	Himinn::IVector2 GetGridPosition() const;

private:
	Himinn::IVector2 m_GridPosition;
	Himinn::IVector2 m_GridSpawnPosition;

	// Movement
	bool m_OnLift;
	bool m_CanMove;
	bool m_IsQbert;
	bool m_EnableMovement;
	float m_MovementTimer;
	float m_MovementDelay;
	float m_LiftTimer;
	float m_LiftDelay;

	// Components
	std::weak_ptr<GridComponent> m_pGridComponent;
	std::weak_ptr<ControllableComponent> m_pControllableComponent;

	void AddToNode();
	void RemoveFromNode();
};