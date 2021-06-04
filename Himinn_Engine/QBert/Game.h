#pragma once
#include "Minigin.h"
#include "PlayerManager.h"

class Game : public Himinn::Minigin
{
public:
	Game();
	virtual void LoadGame() override;
	virtual void Cleanup() override;

private:
	PlayerManager m_PlayerManager;
	std::shared_ptr<GridComponent> m_pGridComponent;
	
	void LoadLevel1();
	void LoadLevel2();
	void LoadLevel3();
};

