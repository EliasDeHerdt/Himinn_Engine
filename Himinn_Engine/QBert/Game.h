#pragma once
#include "Minigin.h"
#include "PlayerManagerComponent.h"


class ManagerObserver;
class LevelManagerComponent;
class PlayerManagerComponent;
class Game : public Himinn::Minigin
{
public:
	Game();
	virtual void LoadGame() override;
	virtual void Cleanup() override;

private:
	std::shared_ptr<ManagerObserver> m_ManagerObserver;
	std::shared_ptr<Himinn::GameObject> m_ManagerObject;
	std::shared_ptr<LevelManagerComponent> m_LevelManagerComponent;
	std::shared_ptr<PlayerManagerComponent> m_PlayerManagerComponent;
};

