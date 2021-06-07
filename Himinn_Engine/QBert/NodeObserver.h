#pragma once
#include "Observer.h"
#include <memory>

enum class NodeObserverEvent
{
	NodeReady,
	NodeNotReady,
};

class GridComponent;
class NodeObserver final : public Himinn::Observer
{
public:
	NodeObserver() = default;
	~NodeObserver() override = default;
	NodeObserver(const NodeObserver& other) = delete;
	NodeObserver& operator=(const NodeObserver& other) = delete;
	NodeObserver(NodeObserver&& other) = delete;
	NodeObserver& operator=(NodeObserver&& other) = delete;

	void OnNotify(Himinn::EventInfo eventInfo, unsigned int event) override;
	bool SetGridComponent(std::weak_ptr<GridComponent> gridComp);

private:
	std::weak_ptr<GridComponent> m_pGridComponent;
};

