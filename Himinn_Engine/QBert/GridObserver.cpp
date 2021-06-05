#include "GridObserver.h"

#include "LevelManagerComponent.h"
void GridObserver::OnNotify(Himinn::EventInfo eventInfo, unsigned int event)
{
	switch (event)
	{
	case (unsigned)GridObserverEvent::GridCompleted:
		if (!m_pLevelManager.expired())
			m_pLevelManager.lock()->LevelCompleted();
		break;
	default: break;
	}
}

bool GridObserver::SetLevelManager(std::weak_ptr<LevelManagerComponent> levelManager)
{
	if (levelManager.expired())
		return false;
	
	m_pLevelManager = levelManager;
	return true;
}
