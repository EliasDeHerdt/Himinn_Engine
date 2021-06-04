#pragma once
#include <memory>
#include <vector>

class GridComponent;

namespace Himinn {
	class GameObject;
}

enum class GameMode
{
	SinglePlayer,
	Coop,
	Versus
};

struct PlayerControls
{
	unsigned int topLeft, topRight, bottomLeft, bottomRight, playerNumber = 0;
	bool controller = false;
};

class PlayerManager
{
public:
	PlayerManager();
	virtual ~PlayerManager() = default;
	PlayerManager(const PlayerManager& other) = delete;
	PlayerManager& operator=(const PlayerManager& other) = delete;
	PlayerManager(PlayerManager&& other) = delete;
	PlayerManager& operator=(PlayerManager&& other) = delete;

	void SetGameMode(GameMode gameMode);
	void MovePlayersToSpawns();
	void SetupManagerForLevel(std::weak_ptr<GridComponent> grid);

private:
	GameMode m_GameMode;
	int m_MaxPlayers;
	bool m_PlayersInitialized;
	std::weak_ptr<GridComponent> m_pGridComponent;
	std::vector<std::pair<std::shared_ptr<Himinn::GameObject>, PlayerControls>> m_Players;

	void AddPlayer(std::shared_ptr<Himinn::GameObject>& player, PlayerControls controls);
};