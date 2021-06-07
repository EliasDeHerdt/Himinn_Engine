#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Component.h"

namespace Himinn {
	class Scene;
	class SubjectComponent;
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

class PlayerObserver;
class GridComponent;
class PlayerManagerComponent : public Himinn::Component
{
public:
	PlayerManagerComponent(const std::weak_ptr<Himinn::GameObject>& owner);
	virtual ~PlayerManagerComponent() override = default;
	PlayerManagerComponent(const PlayerManagerComponent& other) = delete;
	PlayerManagerComponent& operator=(const PlayerManagerComponent& other) = delete;
	PlayerManagerComponent(PlayerManagerComponent&& other) = delete;
	PlayerManagerComponent& operator=(PlayerManagerComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

	void SetGameMode(GameMode gameMode);
	void MovePlayersToSpawns();
	void SetupManagerForLevel(std::weak_ptr<Himinn::Scene> scene, std::weak_ptr<GridComponent> grid);
	void PlayerDied();
	void CleanUp();

private:
	GameMode m_GameMode;
	int m_MaxPlayers;
	int m_LivesPerPlayer;
	bool m_PlayersInitialized;
	std::vector<std::weak_ptr<Himinn::GameObject>> m_Levels;
	std::vector<std::shared_ptr<PlayerObserver>> m_PlayerObservers;
	std::vector<std::pair<std::shared_ptr<Himinn::GameObject>, PlayerControls>> m_Players;

	std::weak_ptr<GridComponent> m_pGridComponent;
	std::weak_ptr<Himinn::SubjectComponent> m_SubjectComponent;
	
	void AddPlayer(std::shared_ptr<Himinn::GameObject>& player, std::string texturePath, PlayerControls controls);
};