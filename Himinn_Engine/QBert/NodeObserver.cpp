#include "NodeObserver.h"


#include "GridComponent.h"
#include "NodeComponent.h"

void NodeObserver::OnNotify(Himinn::EventInfo eventInfo, unsigned event)
{
	switch (event)
	{
		case (unsigned)NodeObserverEvent::NodeReady:
			m_pGridComponent.lock()->UpdateNodeCompletion(true);
			break;
		case (unsigned)NodeObserverEvent::NodeNotReady:
			m_pGridComponent.lock()->UpdateNodeCompletion(false);
			break;
		default: break;
	}
}

bool NodeObserver::SetGridComponent(std::weak_ptr<GridComponent> gridComp)
{
	if (gridComp.expired())
		return false;

	m_pGridComponent = gridComp;
	return true;
}
