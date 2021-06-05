#pragma once
#include <memory>

#include "Component.h"
#include "DataTypes.h"

namespace Himinn {
	class GameObject;
	class SubjectComponent;
}

enum class ScoreGain
{
	ColorChange = 25,
	DiscRemaining = 50,
	Catch = 300,
	DefeatCoily = 500
};

class GridComponent;
class CharacterComponent : public Himinn::Component
{
public:
	CharacterComponent(const std::weak_ptr<Himinn::GameObject>& owner, const std::weak_ptr<GridComponent>& grid, int lives);
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

	int GetLives() const;
	int GetScore() const;

	void Move(Himinn::QBertDirection direction);
	void MoveToSpawn();
	
	void LoseLife();
	void SetLives(int lives);
	void GainScore(ScoreGain score);
	void SetGrid(const std::weak_ptr<GridComponent>& grid);
	void SetGridPosition(int layer, int number);
	void SetGridPosition(Himinn::IVector2 position);
	void SetGridSpawnPosition(int layer, int number);
	void SetGridSpawnPosition(Himinn::IVector2 position);
	Himinn::IVector2 GetGridPosition() const;
private:
	int m_Lives;
	int m_Score;
	Himinn::IVector2 m_GridPosition;
	Himinn::IVector2 m_GridSpawnPosition;

	// Movement
	bool m_CanMove;
	bool m_CanUpgradeNodes;
	float m_MovementTimer;
	float m_MovementDelay;

	// Components
	std::weak_ptr<Himinn::SubjectComponent> m_pSubjectComponent;
	std::weak_ptr<GridComponent> m_pGridComponent;
};