#pragma once
#include <memory>

#include "Observer.h"



enum class ManagerObserverEvent
{
	LevelLoaded,
	PassingPlayers,
	ClearEnemies,
	GameOver
};

class LevelManagerComponent;
class EnemyManagerComponent;
class PlayerManagerComponent;
class ManagerObserver : public Himinn::Observer
{
public:
	ManagerObserver() = default;
	~ManagerObserver() override = default;
	ManagerObserver(const ManagerObserver& other) = delete;
	ManagerObserver& operator=(const ManagerObserver& other) = delete;
	ManagerObserver(ManagerObserver&& other) = delete;
	ManagerObserver& operator=(ManagerObserver&& other) = delete;

	void OnNotify(Himinn::EventInfo eventInfo, unsigned int event) override;

	bool SetLevelManager(std::weak_ptr<LevelManagerComponent> levelManager);
	bool SetEnemyManager(std::weak_ptr<EnemyManagerComponent> enemyManager);
	bool SetPlayerManager(std::weak_ptr<PlayerManagerComponent> playerManager);

private:
	std::weak_ptr<LevelManagerComponent> m_pLevelManager;
	std::weak_ptr<EnemyManagerComponent> m_pEnemyManager;
	std::weak_ptr<PlayerManagerComponent> m_pPlayerManager;
	
};
