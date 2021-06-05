#pragma once
#include "Observer.h"
#include <memory>

enum class GridObserverEvent
{
	GridCompleted
};

class LevelManagerComponent;
class GridObserver : public Himinn::Observer
{
public:
	GridObserver() = default;
	~GridObserver() = default;
	GridObserver(const GridObserver& other) = delete;
	GridObserver& operator=(const GridObserver& other) = delete;
	GridObserver(GridObserver&& other) = delete;
	GridObserver& operator=(GridObserver&& other) = delete;

	void OnNotify(Himinn::EventInfo eventInfo, unsigned int event) override;

	bool SetLevelManager(std::weak_ptr<LevelManagerComponent> levelManager);

private:
	std::weak_ptr<LevelManagerComponent> m_pLevelManager;
};

