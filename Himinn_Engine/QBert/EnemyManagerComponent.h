#pragma once
#include "Component.h"

#include <vector>
#include <memory>

class MoveCommand;
class ControllerComponent;

namespace Himinn {
	class Scene;
	class SubjectComponent;
}

class GridComponent;
class EnemyManagerComponent : public Himinn::Component
{
public:
	EnemyManagerComponent(const std::weak_ptr<Himinn::GameObject>& owner);
	virtual ~EnemyManagerComponent() override = default;
	EnemyManagerComponent(const EnemyManagerComponent& other) = delete;
	EnemyManagerComponent& operator=(const EnemyManagerComponent& other) = delete;
	EnemyManagerComponent(EnemyManagerComponent&& other) = delete;
	EnemyManagerComponent& operator=(EnemyManagerComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

	void SetupManagerForLevel(const std::weak_ptr<Himinn::Scene>& scene, const std::weak_ptr<GridComponent>& grid);
	void SpawnSlickOrSam();
	void SpawnUggOrWrongway();
	void SpawnCoily();
	void ClearEnemies();
	void SetPlayers(const std::vector<std::weak_ptr<ControllerComponent>>& players);
	void SetVersusMode(bool state);

private:
	bool m_Active;
	bool m_VersusMode;
	
	float m_MinSpawnDelaySS;
	float m_MinSpawnDelayUW;
	float m_MaxSpawnDelay;
	
	float m_SpawnDelaySS;
	float m_SpawnDelayUW;
	float m_SpawnDelayCoily;

	float m_TimerSS;
	float m_TimerUW;
	float m_TimerCoily;
	
	float m_StepDelay;

	// DUE TO LACK OF TIME
	std::weak_ptr<MoveCommand> m_pCoilyTopLeftCommand;
	std::weak_ptr<MoveCommand> m_pCoilyTopRightCommand;
	std::weak_ptr<MoveCommand> m_pCoilyBottomLeftCommand;
	std::weak_ptr<MoveCommand> m_pCoilyBottomRightCommand;
	//----------------------

	std::weak_ptr<Himinn::GameObject> m_Coily;
	std::vector<std::weak_ptr<Himinn::GameObject>> m_Enemies;
	std::vector<std::weak_ptr<ControllerComponent>> m_Players;
	
	std::weak_ptr<Himinn::Scene> m_pScene;
	std::weak_ptr<GridComponent> m_pGridComponent;
	std::weak_ptr<Himinn::SubjectComponent> m_SubjectComponent;

	float RandFloat(float min, float max);
	void CheckDeadEnemies();
};