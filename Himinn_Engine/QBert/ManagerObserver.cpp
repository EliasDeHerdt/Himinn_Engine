#include "ManagerObserver.h"

#include "ControllerComponent.h"
#include "PlayerManagerComponent.h"
#include "EnemyManagerComponent.h"
#include "GridComponent.h"
#include "LevelManagerComponent.h"
#include "Scene.h"

void ManagerObserver::OnNotify(Himinn::EventInfo eventInfo, unsigned event)
{
	switch (event)
	{
		case (unsigned)ManagerObserverEvent::LevelLoaded:
		{
			for (auto pointer : eventInfo.pointerInfo)
				if (pointer.expired())
					return;

			if (!m_pPlayerManager.expired())
				m_pPlayerManager.lock()->SetupManagerForLevel(std::static_pointer_cast<Himinn::Scene>(eventInfo.pointerInfo.at(0).lock()), std::static_pointer_cast<GridComponent>(eventInfo.pointerInfo.at(1).lock()));

			if (!m_pEnemyManager.expired())
				m_pEnemyManager.lock()->SetupManagerForLevel(std::static_pointer_cast<Himinn::Scene>(eventInfo.pointerInfo.at(0).lock()), std::static_pointer_cast<GridComponent>(eventInfo.pointerInfo.at(1).lock()));

			break;
		}
		case (unsigned)ManagerObserverEvent::PassingPlayers:
		{
			std::vector<std::weak_ptr<ControllerComponent>> components;
			for (auto pointer : eventInfo.pointerInfo)
			{
				if (pointer.expired())
					return;

				components.push_back(std::static_pointer_cast<ControllerComponent>(pointer.lock()));
			}

			if (!m_pEnemyManager.expired())
				m_pEnemyManager.lock()->SetPlayers(components);
			break;
		}
		case (unsigned)ManagerObserverEvent::ClearEnemies:
		{
			if (!m_pEnemyManager.expired())
				m_pEnemyManager.lock()->ClearEnemies();
			break;
		}
		case (unsigned)ManagerObserverEvent::GameOver:
			if (!m_pPlayerManager.expired())
				m_pPlayerManager.lock()->CleanUp();

			if (!m_pLevelManager.expired())
				m_pLevelManager.lock()->CleanUp();

			break;
		default: break;
	}
}

bool ManagerObserver::SetLevelManager(std::weak_ptr<LevelManagerComponent> levelManager)
{
	if (levelManager.expired())
		return false;

	m_pLevelManager = levelManager;
	return true;
}

bool ManagerObserver::SetEnemyManager(std::weak_ptr<EnemyManagerComponent> enemyManager)
{
	if (enemyManager.expired())
		return false;

	m_pEnemyManager = enemyManager;
	return true;
}

bool ManagerObserver::SetPlayerManager(std::weak_ptr<PlayerManagerComponent> playerManager)
{
	if (playerManager.expired())
		return false;

	m_pPlayerManager = playerManager;
	return true;
}
