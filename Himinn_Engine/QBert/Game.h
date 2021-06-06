#pragma once
#include <memory>

#include "Minigin.h"


namespace Himinn {
	class GameObject;
}

class EnemyManagerComponent;
class ManagerObserver;
class LevelManagerComponent;
class PlayerManagerComponent;
class Game final : public Himinn::Minigin
{
public:
	Game();

	void LoadGame() override;
	void Cleanup() override;

private:
	bool m_ShowHud = true;
	bool m_ShowTutorial = false;
	
	std::shared_ptr<ManagerObserver> m_ManagerObserver;
	std::shared_ptr<Himinn::GameObject> m_ManagerObject;
	std::shared_ptr<LevelManagerComponent> m_LevelManagerComponent;
	std::shared_ptr<EnemyManagerComponent> m_EnemyManagerComponent;
	std::shared_ptr<PlayerManagerComponent> m_PlayerManagerComponent;
};

