#include "ManagerObserver.h"

#include "PlayerManagerComponent.h"
#include "Component.h"
#include "GridComponent.h"
#include "Scene.h"

void ManagerObserver::OnNotify(Himinn::EventInfo eventInfo, unsigned event)
{
	switch (event)
	{
		case (unsigned)ManagerObserverEvent::LevelLoaded:
			for (auto pointer : eventInfo.pointerInfo)
				if (pointer.expired())
					return;

			if (!m_pPlayerManager.expired())
				m_pPlayerManager.lock()->SetupManagerForLevel(std::static_pointer_cast<Himinn::Scene>(eventInfo.pointerInfo.at(0).lock()), std::static_pointer_cast<GridComponent>(eventInfo.pointerInfo.at(1).lock()));
			break;
		case (unsigned)ManagerObserverEvent::GameOver:
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

bool ManagerObserver::SetPlayerManager(std::weak_ptr<PlayerManagerComponent> playerManager)
{
	if (playerManager.expired())
		return false;

	m_pPlayerManager = playerManager;
	return true;
}
