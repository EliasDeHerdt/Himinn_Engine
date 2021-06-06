#pragma once
#include <memory>

#include "Component.h"
#include "DataTypes.h"

class QBertComponent;

namespace Himinn {
	class GameObject;
	class SubjectComponent;
}

class GridComponent;
class CharacterComponent : public Himinn::Component
{
public:
	CharacterComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid);
	virtual ~CharacterComponent() = default;
	CharacterComponent(const CharacterComponent& other) = delete;
	CharacterComponent& operator=(const CharacterComponent& other) = delete;
	CharacterComponent(CharacterComponent&& other) = delete;
	CharacterComponent& operator=(CharacterComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

	void Move(Himinn::QBertDirection direction);
	void MoveToSpawn();
	
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
	bool m_CanMove;
	float m_MovementTimer;
	float m_MovementDelay;

	// Components
	std::weak_ptr<GridComponent> m_pGridComponent;
	std::weak_ptr<QBertComponent> m_pQbertComponent;

	void AddToNode();
	void RemoveFromNode();
};